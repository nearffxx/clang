//===----- CGOpenMPRuntime.h - Interface to OpenMP Runtimes -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This provides a class for OpenMP runtime code generation.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_CODEGEN_CGOPENMPRUNTIME_H
#define LLVM_CLANG_LIB_CODEGEN_CGOPENMPRUNTIME_H

#include "clang/AST/Type.h"
#include "clang/Basic/OpenMPKinds.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/ValueHandle.h"

namespace llvm {
class ArrayType;
class Constant;
class FunctionType;
class GlobalVariable;
class StructType;
class Type;
class Value;
} // namespace llvm

namespace clang {
class Expr;
class GlobalDecl;
class OMPExecutableDirective;
class VarDecl;
class OMPDeclareReductionDecl;
class IdentifierInfo;

namespace CodeGen {
class Address;
class CodeGenFunction;
class CodeGenModule;

/// A basic class for pre|post-action for advanced codegen sequence for OpenMP
/// region.
class PrePostActionTy {
public:
  explicit PrePostActionTy() {}
  virtual void Enter(CodeGenFunction &CGF) {}
  virtual void Exit(CodeGenFunction &CGF) {}
  virtual ~PrePostActionTy() {}
};

/// Class provides a way to call simple version of codegen for OpenMP region, or
/// an advanced with possible pre|post-actions in codegen.
class RegionCodeGenTy final {
  intptr_t CodeGen;
  typedef void (*CodeGenTy)(intptr_t, CodeGenFunction &, PrePostActionTy &);
  CodeGenTy Callback;
  mutable PrePostActionTy *PrePostAction;
  RegionCodeGenTy() = delete;
  RegionCodeGenTy &operator=(const RegionCodeGenTy &) = delete;
  template <typename Callable>
  static void CallbackFn(intptr_t CodeGen, CodeGenFunction &CGF,
                         PrePostActionTy &Action) {
    return (*reinterpret_cast<Callable *>(CodeGen))(CGF, Action);
  }

public:
  template <typename Callable>
  RegionCodeGenTy(
      Callable &&CodeGen,
      typename std::enable_if<
          !std::is_same<typename std::remove_reference<Callable>::type,
                        RegionCodeGenTy>::value>::type * = nullptr)
      : CodeGen(reinterpret_cast<intptr_t>(&CodeGen)),
        Callback(CallbackFn<typename std::remove_reference<Callable>::type>),
        PrePostAction(nullptr) {}
  void setAction(PrePostActionTy &Action) const { PrePostAction = &Action; }
  void operator()(CodeGenFunction &CGF) const;
};

class CGOpenMPRuntime {
protected:
  CodeGenModule &CGM;

  /// \brief Creates offloading entry for the provided entry ID \a ID,
  /// address \a Addr and size \a Size.
  virtual void createOffloadEntry(llvm::Constant *ID, llvm::Constant *Addr,
                                  uint64_t Size);

  /// \brief Helper to emit outlined function for 'target' directive.
  /// \param D Directive to emit.
  /// \param ParentName Name of the function that encloses the target region.
  /// \param OutlinedFn Outlined function value to be defined by this call.
  /// \param OutlinedFnID Outlined function ID value to be defined by this call.
  /// \param IsOffloadEntry True if the outlined function is an offload entry.
  /// \param CodeGen Lambda codegen specific to an accelerator device.
  /// An oulined function may not be an entry if, e.g. the if clause always
  /// evaluates to false.
  virtual void emitTargetOutlinedFunctionHelper(const OMPExecutableDirective &D,
                                                StringRef ParentName,
                                                llvm::Function *&OutlinedFn,
                                                llvm::Constant *&OutlinedFnID,
                                                bool IsOffloadEntry,
                                                const RegionCodeGenTy &CodeGen);

private:
  /// \brief Default const ident_t object used for initialization of all other
  /// ident_t objects.
  llvm::Constant *DefaultOpenMPPSource = nullptr;
  /// \brief Map of flags and corresponding default locations.
  typedef llvm::DenseMap<unsigned, llvm::Value *> OpenMPDefaultLocMapTy;
  OpenMPDefaultLocMapTy OpenMPDefaultLocMap;
  Address getOrCreateDefaultLocation(unsigned Flags);

  llvm::StructType *IdentTy = nullptr;
  /// \brief Map for SourceLocation and OpenMP runtime library debug locations.
  typedef llvm::DenseMap<unsigned, llvm::Value *> OpenMPDebugLocMapTy;
  OpenMPDebugLocMapTy OpenMPDebugLocMap;
  /// \brief The type for a microtask which gets passed to __kmpc_fork_call().
  /// Original representation is:
  /// typedef void (kmpc_micro)(kmp_int32 global_tid, kmp_int32 bound_tid,...);
  llvm::FunctionType *Kmpc_MicroTy = nullptr;
  /// \brief Stores debug location and ThreadID for the function.
  struct DebugLocThreadIdTy {
    llvm::Value *DebugLoc;
    llvm::Value *ThreadID;
  };
  /// \brief Map of local debug location, ThreadId and functions.
  typedef llvm::DenseMap<llvm::Function *, DebugLocThreadIdTy>
      OpenMPLocThreadIDMapTy;
  OpenMPLocThreadIDMapTy OpenMPLocThreadIDMap;
  /// Map of UDRs and corresponding combiner/initializer.
  typedef llvm::DenseMap<const OMPDeclareReductionDecl *,
                         std::pair<llvm::Function *, llvm::Function *>>
      UDRMapTy;
  UDRMapTy UDRMap;
  /// Map of functions and locally defined UDRs.
  typedef llvm::DenseMap<llvm::Function *,
                         SmallVector<const OMPDeclareReductionDecl *, 4>>
      FunctionUDRMapTy;
  FunctionUDRMapTy FunctionUDRMap;
  IdentifierInfo *In = nullptr;
  IdentifierInfo *Out = nullptr;
  IdentifierInfo *Priv = nullptr;
  IdentifierInfo *Orig = nullptr;
  /// \brief Type kmp_critical_name, originally defined as typedef kmp_int32
  /// kmp_critical_name[8];
  llvm::ArrayType *KmpCriticalNameTy;
  /// \brief An ordered map of auto-generated variables to their unique names.
  /// It stores variables with the following names: 1) ".gomp_critical_user_" +
  /// <critical_section_name> + ".var" for "omp critical" directives; 2)
  /// <mangled_name_for_global_var> + ".cache." for cache for threadprivate
  /// variables.
  llvm::StringMap<llvm::AssertingVH<llvm::Constant>, llvm::BumpPtrAllocator>
      InternalVars;
  /// \brief Type typedef kmp_int32 (* kmp_routine_entry_t)(kmp_int32, void *);
  llvm::Type *KmpRoutineEntryPtrTy = nullptr;
  QualType KmpRoutineEntryPtrQTy;
  /// \brief Type typedef struct kmp_task {
  ///    void *              shareds; /**< pointer to block of pointers to
  ///    shared vars   */
  ///    kmp_routine_entry_t routine; /**< pointer to routine to call for
  ///    executing task */
  ///    kmp_int32           part_id; /**< part id for the task */
  ///    kmp_routine_entry_t destructors; /* pointer to function to invoke
  ///    deconstructors of firstprivate C++ objects */
  /// } kmp_task_t;
  QualType KmpTaskTQTy;
  /// \brief Type typedef struct kmp_depend_info {
  ///    kmp_intptr_t               base_addr;
  ///    size_t                     len;
  ///    struct {
  ///             bool                   in:1;
  ///             bool                   out:1;
  ///    } flags;
  /// } kmp_depend_info_t;
  QualType KmpDependInfoTy;
  /// \brief Type struct __tgt_offload_entry{
  ///   void      *addr;       // Pointer to the offload entry info.
  ///                          // (function or global)
  ///   char      *name;       // Name of the function or global.
  ///   size_t     size;       // Size of the entry info (0 if it a function).
  /// };
  QualType TgtOffloadEntryQTy;
  /// struct __tgt_device_image{
  /// void   *ImageStart;       // Pointer to the target code start.
  /// void   *ImageEnd;         // Pointer to the target code end.
  /// // We also add the host entries to the device image, as it may be useful
  /// // for the target runtime to have access to that information.
  /// __tgt_offload_entry  *EntriesBegin;   // Begin of the table with all
  ///                                       // the entries.
  /// __tgt_offload_entry  *EntriesEnd;     // End of the table with all the
  ///                                       // entries (non inclusive).
  /// };
  QualType TgtDeviceImageQTy;
  /// struct __tgt_bin_desc{
  ///   int32_t              NumDevices;      // Number of devices supported.
  ///   __tgt_device_image   *DeviceImages;   // Arrays of device images
  ///                                         // (one per device).
  ///   __tgt_offload_entry  *EntriesBegin;   // Begin of the table with all the
  ///                                         // entries.
  ///   __tgt_offload_entry  *EntriesEnd;     // End of the table with all the
  ///                                         // entries (non inclusive).
  /// };
  QualType TgtBinaryDescriptorQTy;
  /// \brief Entity that registers the offloading constants that were emitted so
  /// far.
  class OffloadEntriesInfoManagerTy {
    CodeGenModule &CGM;

    /// \brief Number of entries registered so far.
    unsigned OffloadingEntriesNum;

  public:
    /// \brief Base class of the entries info.
    class OffloadEntryInfo {
    public:
      /// \brief Kind of a given entry. Currently, only target regions are
      /// supported.
      enum OffloadingEntryInfoKinds : unsigned {
        // Entry is a target region.
        OFFLOAD_ENTRY_INFO_TARGET_REGION = 0,
        // Invalid entry info.
        OFFLOAD_ENTRY_INFO_INVALID = ~0u
      };

      OffloadEntryInfo() : Order(~0u), Kind(OFFLOAD_ENTRY_INFO_INVALID) {}
      explicit OffloadEntryInfo(OffloadingEntryInfoKinds Kind, unsigned Order)
          : Order(Order), Kind(Kind) {}

      bool isValid() const { return Order != ~0u; }
      unsigned getOrder() const { return Order; }
      OffloadingEntryInfoKinds getKind() const { return Kind; }
      static bool classof(const OffloadEntryInfo *Info) { return true; }

    protected:
      // \brief Order this entry was emitted.
      unsigned Order;

      OffloadingEntryInfoKinds Kind;
    };

    /// \brief Return true if a there are no entries defined.
    bool empty() const;
    /// \brief Return number of entries defined so far.
    unsigned size() const { return OffloadingEntriesNum; }
    OffloadEntriesInfoManagerTy(CodeGenModule &CGM)
        : CGM(CGM), OffloadingEntriesNum(0) {}

    ///
    /// Target region entries related.
    ///
    /// \brief Target region entries info.
    class OffloadEntryInfoTargetRegion : public OffloadEntryInfo {
      // \brief Address of the entity that has to be mapped for offloading.
      llvm::Constant *Addr;
      // \brief Address that can be used as the ID of the entry.
      llvm::Constant *ID;

    public:
      OffloadEntryInfoTargetRegion()
          : OffloadEntryInfo(OFFLOAD_ENTRY_INFO_TARGET_REGION, ~0u),
            Addr(nullptr), ID(nullptr) {}
      explicit OffloadEntryInfoTargetRegion(unsigned Order,
                                            llvm::Constant *Addr,
                                            llvm::Constant *ID)
          : OffloadEntryInfo(OFFLOAD_ENTRY_INFO_TARGET_REGION, Order),
            Addr(Addr), ID(ID) {}

      llvm::Constant *getAddress() const { return Addr; }
      llvm::Constant *getID() const { return ID; }
      void setAddress(llvm::Constant *V) {
        assert(!Addr && "Address as been set before!");
        Addr = V;
      }
      void setID(llvm::Constant *V) {
        assert(!ID && "ID as been set before!");
        ID = V;
      }
      static bool classof(const OffloadEntryInfo *Info) {
        return Info->getKind() == OFFLOAD_ENTRY_INFO_TARGET_REGION;
      }
    };
    /// \brief Initialize target region entry.
    void initializeTargetRegionEntryInfo(unsigned DeviceID, unsigned FileID,
                                         StringRef ParentName, unsigned LineNum,
                                         unsigned Order);
    /// \brief Register target region entry.
    void registerTargetRegionEntryInfo(unsigned DeviceID, unsigned FileID,
                                       StringRef ParentName, unsigned LineNum,
                                       llvm::Constant *Addr,
                                       llvm::Constant *ID);
    /// \brief Return true if a target region entry with the provided
    /// information exists.
    bool hasTargetRegionEntryInfo(unsigned DeviceID, unsigned FileID,
                                  StringRef ParentName, unsigned LineNum) const;
    /// brief Applies action \a Action on all registered entries.
    typedef llvm::function_ref<void(unsigned, unsigned, StringRef, unsigned,
                                    OffloadEntryInfoTargetRegion &)>
        OffloadTargetRegionEntryInfoActTy;
    void actOnTargetRegionEntriesInfo(
        const OffloadTargetRegionEntryInfoActTy &Action);

  private:
    // Storage for target region entries kind. The storage is to be indexed by
    // file ID, device ID, parent function name and line number.
    typedef llvm::DenseMap<unsigned, OffloadEntryInfoTargetRegion>
        OffloadEntriesTargetRegionPerLine;
    typedef llvm::StringMap<OffloadEntriesTargetRegionPerLine>
        OffloadEntriesTargetRegionPerParentName;
    typedef llvm::DenseMap<unsigned, OffloadEntriesTargetRegionPerParentName>
        OffloadEntriesTargetRegionPerFile;
    typedef llvm::DenseMap<unsigned, OffloadEntriesTargetRegionPerFile>
        OffloadEntriesTargetRegionPerDevice;
    typedef OffloadEntriesTargetRegionPerDevice OffloadEntriesTargetRegionTy;
    OffloadEntriesTargetRegionTy OffloadEntriesTargetRegion;
  };
  OffloadEntriesInfoManagerTy OffloadEntriesInfoManager;

  /// \brief Creates and registers offloading binary descriptor for the current
  /// compilation unit. The function that does the registration is returned.
  llvm::Function *createOffloadingBinaryDescriptorRegistration();

  /// \brief Creates all the offload entries in the current compilation unit
  /// along with the associated metadata.
  void createOffloadEntriesAndInfoMetadata();

  /// \brief Loads all the offload entries information from the host IR
  /// metadata.
  void loadOffloadInfoMetadata();

  /// \brief Returns __tgt_offload_entry type.
  QualType getTgtOffloadEntryQTy();

  /// \brief Returns __tgt_device_image type.
  QualType getTgtDeviceImageQTy();

  /// \brief Returns __tgt_bin_desc type.
  QualType getTgtBinaryDescriptorQTy();

  /// \brief Start scanning from statement \a S and and emit all target regions
  /// found along the way.
  /// \param S Starting statement.
  /// \param ParentName Name of the function declaration that is being scanned.
  void scanForTargetRegionsFunctions(const Stmt *S, StringRef ParentName);

  /// \brief Build type kmp_routine_entry_t (if not built yet).
  void emitKmpRoutineEntryT(QualType KmpInt32Ty);

  /// \brief Emits object of ident_t type with info for source location.
  /// \param Flags Flags for OpenMP location.
  ///
  llvm::Value *emitUpdateLocation(CodeGenFunction &CGF, SourceLocation Loc,
                                  unsigned Flags = 0);

  /// \brief Returns pointer to ident_t type.
  llvm::Type *getIdentTyPointerTy();

  /// \brief Returns pointer to kmpc_micro type.
  llvm::Type *getKmpc_MicroPointerTy();

  /// \brief Returns specified OpenMP runtime function.
  /// \param Function OpenMP runtime function.
  /// \return Specified function.
  llvm::Constant *createRuntimeFunction(unsigned Function);

  /// \brief Returns __kmpc_for_static_init_* runtime function for the specified
  /// size \a IVSize and sign \a IVSigned.
  llvm::Constant *createForStaticInitFunction(unsigned IVSize, bool IVSigned);

  /// \brief Returns __kmpc_dispatch_init_* runtime function for the specified
  /// size \a IVSize and sign \a IVSigned.
  llvm::Constant *createDispatchInitFunction(unsigned IVSize, bool IVSigned);

  /// \brief Returns __kmpc_dispatch_next_* runtime function for the specified
  /// size \a IVSize and sign \a IVSigned.
  llvm::Constant *createDispatchNextFunction(unsigned IVSize, bool IVSigned);

  /// \brief Returns __kmpc_dispatch_fini_* runtime function for the specified
  /// size \a IVSize and sign \a IVSigned.
  llvm::Constant *createDispatchFiniFunction(unsigned IVSize, bool IVSigned);

  /// \brief If the specified mangled name is not in the module, create and
  /// return threadprivate cache object. This object is a pointer's worth of
  /// storage that's reserved for use by the OpenMP runtime.
  /// \param VD Threadprivate variable.
  /// \return Cache variable for the specified threadprivate.
  llvm::Constant *getOrCreateThreadPrivateCache(const VarDecl *VD);

  /// \brief Emits address of the word in a memory where current thread id is
  /// stored.
  virtual Address emitThreadIDAddress(CodeGenFunction &CGF, SourceLocation Loc);

  /// \brief Gets thread id value for the current thread.
  ///
  llvm::Value *getThreadID(CodeGenFunction &CGF, SourceLocation Loc);

  /// \brief Gets (if variable with the given name already exist) or creates
  /// internal global variable with the specified Name. The created variable has
  /// linkage CommonLinkage by default and is initialized by null value.
  /// \param Ty Type of the global variable. If it is exist already the type
  /// must be the same.
  /// \param Name Name of the variable.
  llvm::Constant *getOrCreateInternalVariable(llvm::Type *Ty,
                                              const llvm::Twine &Name);

  /// \brief Set of threadprivate variables with the generated initializer.
  llvm::SmallPtrSet<const VarDecl *, 4> ThreadPrivateWithDefinition;

  /// \brief Emits initialization code for the threadprivate variables.
  /// \param VDAddr Address of the global variable \a VD.
  /// \param Ctor Pointer to a global init function for \a VD.
  /// \param CopyCtor Pointer to a global copy function for \a VD.
  /// \param Dtor Pointer to a global destructor function for \a VD.
  /// \param Loc Location of threadprivate declaration.
  void emitThreadPrivateVarInit(CodeGenFunction &CGF, Address VDAddr,
                                llvm::Value *Ctor, llvm::Value *CopyCtor,
                                llvm::Value *Dtor, SourceLocation Loc);

  /// \brief Returns corresponding lock object for the specified critical region
  /// name. If the lock object does not exist it is created, otherwise the
  /// reference to the existing copy is returned.
  /// \param CriticalName Name of the critical region.
  ///
  llvm::Value *getCriticalRegionLock(StringRef CriticalName);

public:
  explicit CGOpenMPRuntime(CodeGenModule &CGM);
  virtual ~CGOpenMPRuntime() {}
  virtual void clear();

  /// Emit code for the specified user defined reduction construct.
  virtual void emitUserDefinedReduction(CodeGenFunction *CGF,
                                        const OMPDeclareReductionDecl *D);
  /// Get combiner/initializer for the specified user-defined reduction, if any.
  virtual std::pair<llvm::Function *, llvm::Function *>
  getUserDefinedReduction(const OMPDeclareReductionDecl *D);
  /// \brief Emits outlined function for the specified OpenMP parallel directive
  /// \a D. This outlined function has type void(*)(kmp_int32 *ThreadID,
  /// kmp_int32 BoundID, struct context_vars*).
  /// \param D OpenMP directive.
  /// \param ThreadIDVar Variable for thread id in the current OpenMP region.
  /// \param InnermostKind Kind of innermost directive (for simple directives it
  /// is a directive itself, for combined - its innermost directive).
  /// \param CodeGen Code generation sequence for the \a D directive.
  virtual llvm::Value *emitParallelOrTeamsOutlinedFunction(
      const OMPExecutableDirective &D, const VarDecl *ThreadIDVar,
      OpenMPDirectiveKind InnermostKind, const RegionCodeGenTy &CodeGen);

  /// \brief Emits outlined function for the OpenMP task directive \a D. This
  /// outlined function has type void(*)(kmp_int32 ThreadID, struct task_t*
  /// TaskT).
  /// \param D OpenMP directive.
  /// \param ThreadIDVar Variable for thread id in the current OpenMP region.
  /// \param PartIDVar Variable for partition id in the current OpenMP untied
  /// task region.
  /// \param TaskTVar Variable for task_t argument.
  /// \param InnermostKind Kind of innermost directive (for simple directives it
  /// is a directive itself, for combined - its innermost directive).
  /// \param CodeGen Code generation sequence for the \a D directive.
  /// \param Tied true if task is generated for tied task, false otherwise.
  /// \param NumberOfParts Number of parts in untied task. Ignored for tied
  /// tasks.
  ///
  virtual llvm::Value *emitTaskOutlinedFunction(
      const OMPExecutableDirective &D, const VarDecl *ThreadIDVar,
      const VarDecl *PartIDVar, const VarDecl *TaskTVar,
      OpenMPDirectiveKind InnermostKind, const RegionCodeGenTy &CodeGen,
      bool Tied, unsigned &NumberOfParts);

  /// \brief Cleans up references to the objects in finished function.
  ///
  void functionFinished(CodeGenFunction &CGF);

  /// \brief Emits code for parallel or serial call of the \a OutlinedFn with
  /// variables captured in a record which address is stored in \a
  /// CapturedStruct.
  /// \param OutlinedFn Outlined function to be run in parallel threads. Type of
  /// this function is void(*)(kmp_int32 *, kmp_int32, struct context_vars*).
  /// \param CapturedVars A pointer to the record with the references to
  /// variables used in \a OutlinedFn function.
  /// \param IfCond Condition in the associated 'if' clause, if it was
  /// specified, nullptr otherwise.
  ///
  virtual void emitParallelCall(CodeGenFunction &CGF, SourceLocation Loc,
                                llvm::Value *OutlinedFn,
                                ArrayRef<llvm::Value *> CapturedVars,
                                const Expr *IfCond);

  /// \brief Emits a critical region.
  /// \param CriticalName Name of the critical region.
  /// \param CriticalOpGen Generator for the statement associated with the given
  /// critical region.
  /// \param Hint Value of the 'hint' clause (optional).
  virtual void emitCriticalRegion(CodeGenFunction &CGF, StringRef CriticalName,
                                  const RegionCodeGenTy &CriticalOpGen,
                                  SourceLocation Loc,
                                  const Expr *Hint = nullptr);

  /// \brief Emits a master region.
  /// \param MasterOpGen Generator for the statement associated with the given
  /// master region.
  virtual void emitMasterRegion(CodeGenFunction &CGF,
                                const RegionCodeGenTy &MasterOpGen,
                                SourceLocation Loc);

  /// \brief Emits code for a taskyield directive.
  virtual void emitTaskyieldCall(CodeGenFunction &CGF, SourceLocation Loc);

  /// \brief Emit a taskgroup region.
  /// \param TaskgroupOpGen Generator for the statement associated with the
  /// given taskgroup region.
  virtual void emitTaskgroupRegion(CodeGenFunction &CGF,
                                   const RegionCodeGenTy &TaskgroupOpGen,
                                   SourceLocation Loc);

  /// \brief Emits a single region.
  /// \param SingleOpGen Generator for the statement associated with the given
  /// single region.
  virtual void emitSingleRegion(CodeGenFunction &CGF,
                                const RegionCodeGenTy &SingleOpGen,
                                SourceLocation Loc,
                                ArrayRef<const Expr *> CopyprivateVars,
                                ArrayRef<const Expr *> DestExprs,
                                ArrayRef<const Expr *> SrcExprs,
                                ArrayRef<const Expr *> AssignmentOps);

  /// \brief Emit an ordered region.
  /// \param OrderedOpGen Generator for the statement associated with the given
  /// ordered region.
  virtual void emitOrderedRegion(CodeGenFunction &CGF,
                                 const RegionCodeGenTy &OrderedOpGen,
                                 SourceLocation Loc, bool IsThreads);

  /// \brief Emit an implicit/explicit barrier for OpenMP threads.
  /// \param Kind Directive for which this implicit barrier call must be
  /// generated. Must be OMPD_barrier for explicit barrier generation.
  /// \param EmitChecks true if need to emit checks for cancellation barriers.
  /// \param ForceSimpleCall true simple barrier call must be emitted, false if
  /// runtime class decides which one to emit (simple or with cancellation
  /// checks).
  ///
  virtual void emitBarrierCall(CodeGenFunction &CGF, SourceLocation Loc,
                               OpenMPDirectiveKind Kind,
                               bool EmitChecks = true,
                               bool ForceSimpleCall = false);

  /// \brief Check if the specified \a ScheduleKind is static non-chunked.
  /// This kind of worksharing directive is emitted without outer loop.
  /// \param ScheduleKind Schedule kind specified in the 'schedule' clause.
  /// \param Chunked True if chunk is specified in the clause.
  ///
  virtual bool isStaticNonchunked(OpenMPScheduleClauseKind ScheduleKind,
                                  bool Chunked) const;

  /// \brief Check if the specified \a ScheduleKind is static non-chunked.
  /// This kind of distribute directive is emitted without outer loop.
  /// \param ScheduleKind Schedule kind specified in the 'dist_schedule' clause.
  /// \param Chunked True if chunk is specified in the clause.
  ///
  virtual bool isStaticNonchunked(OpenMPDistScheduleClauseKind ScheduleKind,
                                  bool Chunked) const;

  /// \brief Check if the specified \a ScheduleKind is dynamic.
  /// This kind of worksharing directive is emitted without outer loop.
  /// \param ScheduleKind Schedule Kind specified in the 'schedule' clause.
  ///
  virtual bool isDynamic(OpenMPScheduleClauseKind ScheduleKind) const;

  virtual void emitForDispatchInit(CodeGenFunction &CGF, SourceLocation Loc,
                                   OpenMPScheduleClauseKind SchedKind,
                                   unsigned IVSize, bool IVSigned,
                                   bool Ordered, llvm::Value *UB,
                                   llvm::Value *Chunk = nullptr);

  /// \brief Call the appropriate runtime routine to initialize it before start
  /// of loop.
  ///
  /// Depending on the loop schedule, it is nesessary to call some runtime
  /// routine before start of the OpenMP loop to get the loop upper / lower
  /// bounds \a LB and \a UB and stride \a ST.
  ///
  /// \param CGF Reference to current CodeGenFunction.
  /// \param Loc Clang source location.
  /// \param SchedKind Schedule kind, specified by the 'schedule' clause.
  /// \param IVSize Size of the iteration variable in bits.
  /// \param IVSigned Sign of the interation variable.
  /// \param Ordered true if loop is ordered, false otherwise.
  /// \param IL Address of the output variable in which the flag of the
  /// last iteration is returned.
  /// \param LB Address of the output variable in which the lower iteration
  /// number is returned.
  /// \param UB Address of the output variable in which the upper iteration
  /// number is returned.
  /// \param ST Address of the output variable in which the stride value is
  /// returned nesessary to generated the static_chunked scheduled loop.
  /// \param Chunk Value of the chunk for the static_chunked scheduled loop.
  /// For the default (nullptr) value, the chunk 1 will be used.
  ///
  virtual void emitForStaticInit(CodeGenFunction &CGF, SourceLocation Loc,
                                 OpenMPScheduleClauseKind SchedKind,
                                 unsigned IVSize, bool IVSigned, bool Ordered,
                                 Address IL, Address LB,
                                 Address UB, Address ST,
                                 llvm::Value *Chunk = nullptr);

  ///
  /// \param CGF Reference to current CodeGenFunction.
  /// \param Loc Clang source location.
  /// \param SchedKind Schedule kind, specified by the 'dist_schedule' clause.
  /// \param IVSize Size of the iteration variable in bits.
  /// \param IVSigned Sign of the interation variable.
  /// \param Ordered true if loop is ordered, false otherwise.
  /// \param IL Address of the output variable in which the flag of the
  /// last iteration is returned.
  /// \param LB Address of the output variable in which the lower iteration
  /// number is returned.
  /// \param UB Address of the output variable in which the upper iteration
  /// number is returned.
  /// \param ST Address of the output variable in which the stride value is
  /// returned nesessary to generated the static_chunked scheduled loop.
  /// \param Chunk Value of the chunk for the static_chunked scheduled loop.
  /// For the default (nullptr) value, the chunk 1 will be used.
  ///
  virtual void emitDistributeStaticInit(CodeGenFunction &CGF, SourceLocation Loc,
                                        OpenMPDistScheduleClauseKind SchedKind,
                                        unsigned IVSize, bool IVSigned,
                                        bool Ordered, Address IL, Address LB,
                                        Address UB, Address ST,
                                        llvm::Value *Chunk = nullptr);

  /// \brief Call the appropriate runtime routine to notify that we finished
  /// iteration of the ordered loop with the dynamic scheduling.
  ///
  /// \param CGF Reference to current CodeGenFunction.
  /// \param Loc Clang source location.
  /// \param IVSize Size of the iteration variable in bits.
  /// \param IVSigned Sign of the interation variable.
  ///
  virtual void emitForOrderedIterationEnd(CodeGenFunction &CGF,
                                          SourceLocation Loc, unsigned IVSize,
                                          bool IVSigned);

  /// \brief Call the appropriate runtime routine to notify that we finished
  /// all the work with current loop.
  ///
  /// \param CGF Reference to current CodeGenFunction.
  /// \param Loc Clang source location.
  ///
  virtual void emitForStaticFinish(CodeGenFunction &CGF, SourceLocation Loc);

  /// Call __kmpc_dispatch_next(
  ///          ident_t *loc, kmp_int32 tid, kmp_int32 *p_lastiter,
  ///          kmp_int[32|64] *p_lower, kmp_int[32|64] *p_upper,
  ///          kmp_int[32|64] *p_stride);
  /// \param IVSize Size of the iteration variable in bits.
  /// \param IVSigned Sign of the interation variable.
  /// \param IL Address of the output variable in which the flag of the
  /// last iteration is returned.
  /// \param LB Address of the output variable in which the lower iteration
  /// number is returned.
  /// \param UB Address of the output variable in which the upper iteration
  /// number is returned.
  /// \param ST Address of the output variable in which the stride value is
  /// returned.
  virtual llvm::Value *emitForNext(CodeGenFunction &CGF, SourceLocation Loc,
                                   unsigned IVSize, bool IVSigned,
                                   Address IL, Address LB,
                                   Address UB, Address ST);

  /// \brief Emits call to void __kmpc_push_num_threads(ident_t *loc, kmp_int32
  /// global_tid, kmp_int32 num_threads) to generate code for 'num_threads'
  /// clause.
  /// \param NumThreads An integer value of threads.
  virtual void emitNumThreadsClause(CodeGenFunction &CGF,
                                    llvm::Value *NumThreads,
                                    SourceLocation Loc);

  /// \brief Emit call to void __kmpc_push_proc_bind(ident_t *loc, kmp_int32
  /// global_tid, int proc_bind) to generate code for 'proc_bind' clause.
  virtual void emitProcBindClause(CodeGenFunction &CGF,
                                  OpenMPProcBindClauseKind ProcBind,
                                  SourceLocation Loc);

  /// \brief Returns address of the threadprivate variable for the current
  /// thread.
  /// \param VD Threadprivate variable.
  /// \param VDAddr Address of the global variable \a VD.
  /// \param Loc Location of the reference to threadprivate var.
  /// \return Address of the threadprivate variable for the current thread.
  virtual Address getAddrOfThreadPrivate(CodeGenFunction &CGF,
                                         const VarDecl *VD,
                                         Address VDAddr,
                                         SourceLocation Loc);

  /// \brief Emit a code for initialization of threadprivate variable. It emits
  /// a call to runtime library which adds initial value to the newly created
  /// threadprivate variable (if it is not constant) and registers destructor
  /// for the variable (if any).
  /// \param VD Threadprivate variable.
  /// \param VDAddr Address of the global variable \a VD.
  /// \param Loc Location of threadprivate declaration.
  /// \param PerformInit true if initialization expression is not constant.
  virtual llvm::Function *
  emitThreadPrivateVarDefinition(const VarDecl *VD, Address VDAddr,
                                 SourceLocation Loc, bool PerformInit,
                                 CodeGenFunction *CGF = nullptr);

  /// \brief Emit flush of the variables specified in 'omp flush' directive.
  /// \param Vars List of variables to flush.
  virtual void emitFlush(CodeGenFunction &CGF, ArrayRef<const Expr *> Vars,
                         SourceLocation Loc);

  /// \brief Emit task region for the task directive. The task region is
  /// emitted in several steps:
  /// 1. Emit a call to kmp_task_t *__kmpc_omp_task_alloc(ident_t *, kmp_int32
  /// gtid, kmp_int32 flags, size_t sizeof_kmp_task_t, size_t sizeof_shareds,
  /// kmp_routine_entry_t *task_entry). Here task_entry is a pointer to the
  /// function:
  /// kmp_int32 .omp_task_entry.(kmp_int32 gtid, kmp_task_t *tt) {
  ///   TaskFunction(gtid, tt->part_id, tt->shareds);
  ///   return 0;
  /// }
  /// 2. Copy a list of shared variables to field shareds of the resulting
  /// structure kmp_task_t returned by the previous call (if any).
  /// 3. Copy a pointer to destructions function to field destructions of the
  /// resulting structure kmp_task_t.
  /// 4. Emit a call to kmp_int32 __kmpc_omp_task(ident_t *, kmp_int32 gtid,
  /// kmp_task_t *new_task), where new_task is a resulting structure from
  /// previous items.
  /// \param D Current task directive.
  /// \param Tied true if the task is tied (the task is tied to the thread that
  /// can suspend its task region), false - untied (the task is not tied to any
  /// thread).
  /// \param NumberOfParts Number of parts for untied task.
  /// \param Final Contains either constant bool value, or llvm::Value * of i1
  /// type for final clause. If the value is true, the task forces all of its
  /// child tasks to become final and included tasks.
  /// \param TaskFunction An LLVM function with type void (*)(i32 /*gtid*/, i32
  /// /*part_id*/, captured_struct */*__context*/);
  /// \param SharedsTy A type which contains references the shared variables.
  /// \param Shareds Context with the list of shared variables from the \p
  /// TaskFunction.
  /// \param IfCond Not a nullptr if 'if' clause was specified, nullptr
  /// otherwise.
  /// \param PrivateVars List of references to private variables for the task
  /// directive.
  /// \param PrivateCopies List of private copies for each private variable in
  /// \p PrivateVars.
  /// \param FirstprivateVars List of references to private variables for the
  /// task directive.
  /// \param FirstprivateCopies List of private copies for each private variable
  /// in \p FirstprivateVars.
  /// \param FirstprivateInits List of references to auto generated variables
  /// used for initialization of a single array element. Used if firstprivate
  /// variable is of array type.
  /// \param Dependences List of dependences for the 'task' construct, including
  /// original expression and dependency type.
  virtual void emitTaskCall(
      CodeGenFunction &CGF, SourceLocation Loc, const OMPExecutableDirective &D,
      bool Tied, llvm::PointerIntPair<llvm::Value *, 1, bool> Final,
      unsigned NumberOfParts, llvm::Value *TaskFunction, QualType SharedsTy,
      Address Shareds, const Expr *IfCond, ArrayRef<const Expr *> PrivateVars,
      ArrayRef<const Expr *> PrivateCopies,
      ArrayRef<const Expr *> FirstprivateVars,
      ArrayRef<const Expr *> FirstprivateCopies,
      ArrayRef<const Expr *> FirstprivateInits,
      ArrayRef<std::pair<OpenMPDependClauseKind, const Expr *>> Dependences);

  /// \brief Emit code for the directive that does not require outlining.
  ///
  /// \param InnermostKind Kind of innermost directive (for simple directives it
  /// is a directive itself, for combined - its innermost directive).
  /// \param CodeGen Code generation sequence for the \a D directive.
  /// \param HasCancel true if region has inner cancel directive, false
  /// otherwise.
  virtual void emitInlinedDirective(CodeGenFunction &CGF,
                                    OpenMPDirectiveKind InnermostKind,
                                    const RegionCodeGenTy &CodeGen,
                                    bool HasCancel = false);
  /// \brief Emit a code for reduction clause. Next code should be emitted for
  /// reduction:
  /// \code
  ///
  /// static kmp_critical_name lock = { 0 };
  ///
  /// void reduce_func(void *lhs[<n>], void *rhs[<n>]) {
  ///  ...
  ///  *(Type<i>*)lhs[i] = RedOp<i>(*(Type<i>*)lhs[i], *(Type<i>*)rhs[i]);
  ///  ...
  /// }
  ///
  /// ...
  /// void *RedList[<n>] = {&<RHSExprs>[0], ..., &<RHSExprs>[<n>-1]};
  /// switch (__kmpc_reduce{_nowait}(<loc>, <gtid>, <n>, sizeof(RedList),
  /// RedList, reduce_func, &<lock>)) {
  /// case 1:
  ///  ...
  ///  <LHSExprs>[i] = RedOp<i>(*<LHSExprs>[i], *<RHSExprs>[i]);
  ///  ...
  /// __kmpc_end_reduce{_nowait}(<loc>, <gtid>, &<lock>);
  /// break;
  /// case 2:
  ///  ...
  ///  Atomic(<LHSExprs>[i] = RedOp<i>(*<LHSExprs>[i], *<RHSExprs>[i]));
  ///  ...
  /// break;
  /// default:;
  /// }
  /// \endcode
  ///
  /// \param Privates List of private copies for original reduction arguments.
  /// \param LHSExprs List of LHS in \a ReductionOps reduction operations.
  /// \param RHSExprs List of RHS in \a ReductionOps reduction operations.
  /// \param ReductionOps List of reduction operations in form 'LHS binop RHS'
  /// or 'operator binop(LHS, RHS)'.
  /// \param WithNowait true if parent directive has also nowait clause, false
  /// otherwise.
  virtual void emitReduction(CodeGenFunction &CGF, SourceLocation Loc,
                             ArrayRef<const Expr *> Privates,
                             ArrayRef<const Expr *> LHSExprs,
                             ArrayRef<const Expr *> RHSExprs,
                             ArrayRef<const Expr *> ReductionOps,
                             bool WithNowait, bool SimpleReduction);

  /// \brief Emit code for 'taskwait' directive.
  virtual void emitTaskwaitCall(CodeGenFunction &CGF, SourceLocation Loc);

  /// \brief Emit code for 'cancellation point' construct.
  /// \param CancelRegion Region kind for which the cancellation point must be
  /// emitted.
  ///
  virtual void emitCancellationPointCall(CodeGenFunction &CGF,
                                         SourceLocation Loc,
                                         OpenMPDirectiveKind CancelRegion);

  /// \brief Emit code for 'cancel' construct.
  /// \param IfCond Condition in the associated 'if' clause, if it was
  /// specified, nullptr otherwise.
  /// \param CancelRegion Region kind for which the cancel must be emitted.
  ///
  virtual void emitCancelCall(CodeGenFunction &CGF, SourceLocation Loc,
                              const Expr *IfCond,
                              OpenMPDirectiveKind CancelRegion);

  /// \brief Emit outilined function for 'target' directive.
  /// \param D Directive to emit.
  /// \param ParentName Name of the function that encloses the target region.
  /// \param OutlinedFn Outlined function value to be defined by this call.
  /// \param OutlinedFnID Outlined function ID value to be defined by this call.
  /// \param IsOffloadEntry True if the outlined function is an offload entry.
  /// \param CodeGen Code generation sequence for the \a D directive.
  /// An oulined function may not be an entry if, e.g. the if clause always
  /// evaluates to false.
  virtual void emitTargetOutlinedFunction(const OMPExecutableDirective &D,
                                          StringRef ParentName,
                                          llvm::Function *&OutlinedFn,
                                          llvm::Constant *&OutlinedFnID,
                                          bool IsOffloadEntry,
                                          const RegionCodeGenTy &CodeGen);

  /// \brief Emit the target offloading code associated with \a D. The emitted
  /// code attempts offloading the execution to the device, an the event of
  /// a failure it executes the host version outlined in \a OutlinedFn.
  /// \param D Directive to emit.
  /// \param OutlinedFn Host version of the code to be offloaded.
  /// \param OutlinedFnID ID of host version of the code to be offloaded.
  /// \param IfCond Expression evaluated in if clause associated with the target
  /// directive, or null if no if clause is used.
  /// \param Device Expression evaluated in device clause associated with the
  /// target directive, or null if no device clause is used.
  /// \param CapturedVars Values captured in the current region.
  virtual void emitTargetCall(CodeGenFunction &CGF,
                              const OMPExecutableDirective &D,
                              llvm::Value *OutlinedFn,
                              llvm::Value *OutlinedFnID, const Expr *IfCond,
                              const Expr *Device,
                              ArrayRef<llvm::Value *> CapturedVars);

  /// \brief Emit the target regions enclosed in \a GD function definition or
  /// the function itself in case it is a valid device function. Returns true if
  /// \a GD was dealt with successfully.
  /// \param GD Function to scan.
  virtual bool emitTargetFunctions(GlobalDecl GD);

  /// \brief Emit the global variable if it is a valid device global variable.
  /// Returns true if \a GD was dealt with successfully.
  /// \param GD Variable declaration to emit.
  virtual bool emitTargetGlobalVariable(GlobalDecl GD);

  /// \brief Emit the global \a GD if it is meaningful for the target. Returns
  /// if it was emitted succesfully.
  /// \param GD Global to scan.
  virtual bool emitTargetGlobal(GlobalDecl GD);

  /// \brief Creates the offloading descriptor in the event any target region
  /// was emitted in the current module and return the function that registers
  /// it.
  virtual llvm::Function *emitRegistrationFunction();

  /// \brief Emits code for teams call of the \a OutlinedFn with
  /// variables captured in a record which address is stored in \a
  /// CapturedStruct.
  /// \param OutlinedFn Outlined function to be run by team masters. Type of
  /// this function is void(*)(kmp_int32 *, kmp_int32, struct context_vars*).
  /// \param CapturedVars A pointer to the record with the references to
  /// variables used in \a OutlinedFn function.
  ///
  virtual void emitTeamsCall(CodeGenFunction &CGF,
                             const OMPExecutableDirective &D,
                             SourceLocation Loc, llvm::Value *OutlinedFn,
                             ArrayRef<llvm::Value *> CapturedVars);

  /// \brief Emits call to void __kmpc_push_num_teams(ident_t *loc, kmp_int32
  /// global_tid, kmp_int32 num_teams, kmp_int32 thread_limit) to generate code
  /// for num_teams clause.
  /// \param NumTeams An integer expression of teams.
  /// \param ThreadLimit An integer expression of threads.
  virtual void emitNumTeamsClause(CodeGenFunction &CGF, const Expr *NumTeams,
                                  const Expr *ThreadLimit, SourceLocation Loc);
};

} // namespace CodeGen
} // namespace clang

#endif
