/*===---- avx512vldqintrin.h - AVX512VL and AVX512DQ intrinsics ------------===
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */

#ifndef __IMMINTRIN_H
#error "Never use <avx512vldqintrin.h> directly; include <immintrin.h> instead."
#endif

#ifndef __AVX512VLDQINTRIN_H
#define __AVX512VLDQINTRIN_H

/* Define the default attributes for the functions in this file. */
#define __DEFAULT_FN_ATTRS __attribute__((__always_inline__, __nodebug__, __target__("avx512vl,avx512dq")))

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mullo_epi64 (__m256i __A, __m256i __B) {
  return (__m256i) ((__v4di) __A * (__v4di) __B);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_mullo_epi64 (__m256i __W, __mmask8 __U, __m256i __A, __m256i __B) {
  return (__m256i) __builtin_ia32_pmullq256_mask ((__v4di) __A,
              (__v4di) __B,
              (__v4di) __W,
              (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_mullo_epi64 (__mmask8 __U, __m256i __A, __m256i __B) {
  return (__m256i) __builtin_ia32_pmullq256_mask ((__v4di) __A,
              (__v4di) __B,
              (__v4di)
              _mm256_setzero_si256 (),
              (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mullo_epi64 (__m128i __A, __m128i __B) {
  return (__m128i) ((__v2di) __A * (__v2di) __B);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_mullo_epi64 (__m128i __W, __mmask8 __U, __m128i __A, __m128i __B) {
  return (__m128i) __builtin_ia32_pmullq128_mask ((__v2di) __A,
              (__v2di) __B,
              (__v2di) __W,
              (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_mullo_epi64 (__mmask8 __U, __m128i __A, __m128i __B) {
  return (__m128i) __builtin_ia32_pmullq128_mask ((__v2di) __A,
              (__v2di) __B,
              (__v2di)
              _mm_setzero_si128 (),
              (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_andnot_pd (__m256d __W, __mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_andnpd256_mask ((__v4df) __A,
              (__v4df) __B,
              (__v4df) __W,
              (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_andnot_pd (__mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_andnpd256_mask ((__v4df) __A,
              (__v4df) __B,
              (__v4df)
              _mm256_setzero_pd (),
              (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_andnot_pd (__m128d __W, __mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_andnpd128_mask ((__v2df) __A,
              (__v2df) __B,
              (__v2df) __W,
              (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_andnot_pd (__mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_andnpd128_mask ((__v2df) __A,
              (__v2df) __B,
              (__v2df)
              _mm_setzero_pd (),
              (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_mask_andnot_ps (__m256 __W, __mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_andnps256_mask ((__v8sf) __A,
             (__v8sf) __B,
             (__v8sf) __W,
             (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_maskz_andnot_ps (__mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_andnps256_mask ((__v8sf) __A,
             (__v8sf) __B,
             (__v8sf)
             _mm256_setzero_ps (),
             (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_andnot_ps (__m128 __W, __mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_andnps128_mask ((__v4sf) __A,
             (__v4sf) __B,
             (__v4sf) __W,
             (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_andnot_ps (__mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_andnps128_mask ((__v4sf) __A,
             (__v4sf) __B,
             (__v4sf)
             _mm_setzero_ps (),
             (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_and_pd (__m256d __W, __mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_andpd256_mask ((__v4df) __A,
             (__v4df) __B,
             (__v4df) __W,
             (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_and_pd (__mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_andpd256_mask ((__v4df) __A,
             (__v4df) __B,
             (__v4df)
             _mm256_setzero_pd (),
             (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_and_pd (__m128d __W, __mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_andpd128_mask ((__v2df) __A,
             (__v2df) __B,
             (__v2df) __W,
             (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_and_pd (__mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_andpd128_mask ((__v2df) __A,
             (__v2df) __B,
             (__v2df)
             _mm_setzero_pd (),
             (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_mask_and_ps (__m256 __W, __mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_andps256_mask ((__v8sf) __A,
            (__v8sf) __B,
            (__v8sf) __W,
            (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_maskz_and_ps (__mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_andps256_mask ((__v8sf) __A,
            (__v8sf) __B,
            (__v8sf)
            _mm256_setzero_ps (),
            (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_and_ps (__m128 __W, __mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_andps128_mask ((__v4sf) __A,
            (__v4sf) __B,
            (__v4sf) __W,
            (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_and_ps (__mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_andps128_mask ((__v4sf) __A,
            (__v4sf) __B,
            (__v4sf)
            _mm_setzero_ps (),
            (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_xor_pd (__m256d __W, __mmask8 __U, __m256d __A,
        __m256d __B) {
  return (__m256d) __builtin_ia32_xorpd256_mask ((__v4df) __A,
             (__v4df) __B,
             (__v4df) __W,
             (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_xor_pd (__mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_xorpd256_mask ((__v4df) __A,
             (__v4df) __B,
             (__v4df)
             _mm256_setzero_pd (),
             (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_xor_pd (__m128d __W, __mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_xorpd128_mask ((__v2df) __A,
             (__v2df) __B,
             (__v2df) __W,
             (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_xor_pd (__mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_xorpd128_mask ((__v2df) __A,
             (__v2df) __B,
             (__v2df)
             _mm_setzero_pd (),
             (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_mask_xor_ps (__m256 __W, __mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_xorps256_mask ((__v8sf) __A,
            (__v8sf) __B,
            (__v8sf) __W,
            (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_maskz_xor_ps (__mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_xorps256_mask ((__v8sf) __A,
            (__v8sf) __B,
            (__v8sf)
            _mm256_setzero_ps (),
            (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_xor_ps (__m128 __W, __mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_xorps128_mask ((__v4sf) __A,
            (__v4sf) __B,
            (__v4sf) __W,
            (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_xor_ps (__mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_xorps128_mask ((__v4sf) __A,
            (__v4sf) __B,
            (__v4sf)
            _mm_setzero_ps (),
            (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_or_pd (__m256d __W, __mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_orpd256_mask ((__v4df) __A,
            (__v4df) __B,
            (__v4df) __W,
            (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_or_pd (__mmask8 __U, __m256d __A, __m256d __B) {
  return (__m256d) __builtin_ia32_orpd256_mask ((__v4df) __A,
            (__v4df) __B,
            (__v4df)
            _mm256_setzero_pd (),
            (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_or_pd (__m128d __W, __mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_orpd128_mask ((__v2df) __A,
            (__v2df) __B,
            (__v2df) __W,
            (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_or_pd (__mmask8 __U, __m128d __A, __m128d __B) {
  return (__m128d) __builtin_ia32_orpd128_mask ((__v2df) __A,
            (__v2df) __B,
            (__v2df)
            _mm_setzero_pd (),
            (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_mask_or_ps (__m256 __W, __mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_orps256_mask ((__v8sf) __A,
                 (__v8sf) __B,
                 (__v8sf) __W,
                 (__mmask8) __U);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_maskz_or_ps (__mmask8 __U, __m256 __A, __m256 __B) {
  return (__m256) __builtin_ia32_orps256_mask ((__v8sf) __A,
                 (__v8sf) __B,
                 (__v8sf)
                 _mm256_setzero_ps (),
                 (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_or_ps (__m128 __W, __mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_orps128_mask ((__v4sf) __A,
                 (__v4sf) __B,
                 (__v4sf) __W,
                 (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_or_ps (__mmask8 __U, __m128 __A, __m128 __B) {
  return (__m128) __builtin_ia32_orps128_mask ((__v4sf) __A,
                 (__v4sf) __B,
                 (__v4sf)
                 _mm_setzero_ps (),
                 (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvtpd_epi64 (__m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2qq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvtpd_epi64 (__m128i __W, __mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2qq128_mask ((__v2df) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvtpd_epi64 (__mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2qq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvtpd_epi64 (__m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2qq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvtpd_epi64 (__m256i __W, __mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2qq256_mask ((__v4df) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvtpd_epi64 (__mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2qq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvtpd_epu64 (__m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2uqq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvtpd_epu64 (__m128i __W, __mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2uqq128_mask ((__v2df) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvtpd_epu64 (__mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvtpd2uqq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvtpd_epu64 (__m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2uqq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvtpd_epu64 (__m256i __W, __mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2uqq256_mask ((__v4df) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvtpd_epu64 (__mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvtpd2uqq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvtps_epi64 (__m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2qq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvtps_epi64 (__m128i __W, __mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2qq128_mask ((__v4sf) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvtps_epi64 (__mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2qq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvtps_epi64 (__m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2qq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvtps_epi64 (__m256i __W, __mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2qq256_mask ((__v4sf) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvtps_epi64 (__mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2qq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvtps_epu64 (__m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2uqq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvtps_epu64 (__m128i __W, __mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2uqq128_mask ((__v4sf) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvtps_epu64 (__mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvtps2uqq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvtps_epu64 (__m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2uqq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvtps_epu64 (__m256i __W, __mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2uqq256_mask ((__v4sf) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvtps_epu64 (__mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvtps2uqq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_cvtepi64_pd (__m128i __A) {
  return (__m128d) __builtin_ia32_cvtqq2pd128_mask ((__v2di) __A,
                (__v2df) _mm_setzero_pd(),
                (__mmask8) -1);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_cvtepi64_pd (__m128d __W, __mmask8 __U, __m128i __A) {
  return (__m128d) __builtin_ia32_cvtqq2pd128_mask ((__v2di) __A,
                (__v2df) __W,
                (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_cvtepi64_pd (__mmask8 __U, __m128i __A) {
  return (__m128d) __builtin_ia32_cvtqq2pd128_mask ((__v2di) __A,
                (__v2df) _mm_setzero_pd(),
                (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_cvtepi64_pd (__m256i __A) {
  return (__m256d) __builtin_ia32_cvtqq2pd256_mask ((__v4di) __A,
                (__v4df) _mm256_setzero_pd(),
                (__mmask8) -1);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_cvtepi64_pd (__m256d __W, __mmask8 __U, __m256i __A) {
  return (__m256d) __builtin_ia32_cvtqq2pd256_mask ((__v4di) __A,
                (__v4df) __W,
                (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_cvtepi64_pd (__mmask8 __U, __m256i __A) {
  return (__m256d) __builtin_ia32_cvtqq2pd256_mask ((__v4di) __A,
                (__v4df) _mm256_setzero_pd(),
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_cvtepi64_ps (__m128i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps128_mask ((__v2di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) -1);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_cvtepi64_ps (__m128 __W, __mmask8 __U, __m128i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps128_mask ((__v2di) __A,
                (__v4sf) __W,
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_cvtepi64_ps (__mmask8 __U, __m128i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps128_mask ((__v2di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_cvtepi64_ps (__m256i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps256_mask ((__v4di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) -1);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_mask_cvtepi64_ps (__m128 __W, __mmask8 __U, __m256i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps256_mask ((__v4di) __A,
                (__v4sf) __W,
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_maskz_cvtepi64_ps (__mmask8 __U, __m256i __A) {
  return (__m128) __builtin_ia32_cvtqq2ps256_mask ((__v4di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvttpd_epi64 (__m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2qq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvttpd_epi64 (__m128i __W, __mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2qq128_mask ((__v2df) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvttpd_epi64 (__mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2qq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvttpd_epi64 (__m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2qq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvttpd_epi64 (__m256i __W, __mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2qq256_mask ((__v4df) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvttpd_epi64 (__mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2qq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvttpd_epu64 (__m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2uqq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvttpd_epu64 (__m128i __W, __mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2uqq128_mask ((__v2df) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvttpd_epu64 (__mmask8 __U, __m128d __A) {
  return (__m128i) __builtin_ia32_cvttpd2uqq128_mask ((__v2df) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvttpd_epu64 (__m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2uqq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvttpd_epu64 (__m256i __W, __mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2uqq256_mask ((__v4df) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvttpd_epu64 (__mmask8 __U, __m256d __A) {
  return (__m256i) __builtin_ia32_cvttpd2uqq256_mask ((__v4df) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvttps_epi64 (__m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2qq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvttps_epi64 (__m128i __W, __mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2qq128_mask ((__v4sf) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvttps_epi64 (__mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2qq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvttps_epi64 (__m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2qq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvttps_epi64 (__m256i __W, __mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2qq256_mask ((__v4sf) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvttps_epi64 (__mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2qq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_cvttps_epu64 (__m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2uqq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_cvttps_epu64 (__m128i __W, __mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2uqq128_mask ((__v4sf) __A,
                (__v2di) __W,
                (__mmask8) __U);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_cvttps_epu64 (__mmask8 __U, __m128 __A) {
  return (__m128i) __builtin_ia32_cvttps2uqq128_mask ((__v4sf) __A,
                (__v2di) _mm_setzero_si128(),
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_cvttps_epu64 (__m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2uqq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) -1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_cvttps_epu64 (__m256i __W, __mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2uqq256_mask ((__v4sf) __A,
                (__v4di) __W,
                (__mmask8) __U);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_cvttps_epu64 (__mmask8 __U, __m128 __A) {
  return (__m256i) __builtin_ia32_cvttps2uqq256_mask ((__v4sf) __A,
                (__v4di) _mm256_setzero_si256(),
                (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_cvtepu64_pd (__m128i __A) {
  return (__m128d) __builtin_ia32_cvtuqq2pd128_mask ((__v2di) __A,
                (__v2df) _mm_setzero_pd(),
                (__mmask8) -1);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_mask_cvtepu64_pd (__m128d __W, __mmask8 __U, __m128i __A) {
  return (__m128d) __builtin_ia32_cvtuqq2pd128_mask ((__v2di) __A,
                (__v2df) __W,
                (__mmask8) __U);
}

static __inline__ __m128d __DEFAULT_FN_ATTRS
_mm_maskz_cvtepu64_pd (__mmask8 __U, __m128i __A) {
  return (__m128d) __builtin_ia32_cvtuqq2pd128_mask ((__v2di) __A,
                (__v2df) _mm_setzero_pd(),
                (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_cvtepu64_pd (__m256i __A) {
  return (__m256d) __builtin_ia32_cvtuqq2pd256_mask ((__v4di) __A,
                (__v4df) _mm256_setzero_pd(),
                (__mmask8) -1);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_cvtepu64_pd (__m256d __W, __mmask8 __U, __m256i __A) {
  return (__m256d) __builtin_ia32_cvtuqq2pd256_mask ((__v4di) __A,
                (__v4df) __W,
                (__mmask8) __U);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_cvtepu64_pd (__mmask8 __U, __m256i __A) {
  return (__m256d) __builtin_ia32_cvtuqq2pd256_mask ((__v4di) __A,
                (__v4df) _mm256_setzero_pd(),
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_cvtepu64_ps (__m128i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps128_mask ((__v2di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) -1);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_mask_cvtepu64_ps (__m128 __W, __mmask8 __U, __m128i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps128_mask ((__v2di) __A,
                (__v4sf) __W,
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm_maskz_cvtepu64_ps (__mmask8 __U, __m128i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps128_mask ((__v2di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_cvtepu64_ps (__m256i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps256_mask ((__v4di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) -1);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_mask_cvtepu64_ps (__m128 __W, __mmask8 __U, __m256i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps256_mask ((__v4di) __A,
                (__v4sf) __W,
                (__mmask8) __U);
}

static __inline__ __m128 __DEFAULT_FN_ATTRS
_mm256_maskz_cvtepu64_ps (__mmask8 __U, __m256i __A) {
  return (__m128) __builtin_ia32_cvtuqq2ps256_mask ((__v4di) __A,
                (__v4sf) _mm_setzero_ps(),
                (__mmask8) __U);
}

#define _mm_range_pd(__A, __B, __C) __extension__ ({                         \
  (__m128d) __builtin_ia32_rangepd128_mask ((__v2df) __A, (__v2df) __B, __C, \
                (__v2df) _mm_setzero_pd(), (__mmask8) -1); })

#define _mm_mask_range_pd(__W, __U, __A, __B, __C) __extension__ ({          \
  (__m128d) __builtin_ia32_rangepd128_mask ((__v2df) __A, (__v2df) __B, __C, \
                (__v2df) __W, (__mmask8) __U); })

#define _mm_maskz_range_pd(__U, __A, __B, __C) __extension__ ({              \
  (__m128d) __builtin_ia32_rangepd128_mask ((__v2df) __A, (__v2df) __B, __C, \
                (__v2df) _mm_setzero_pd(), (__mmask8) __U); })

#define _mm256_range_pd(__A, __B, __C) __extension__ ({                      \
  (__m256d) __builtin_ia32_rangepd256_mask ((__v4df) __A, (__v4df) __B, __C, \
                (__v4df) _mm256_setzero_pd(), (__mmask8) -1); })

#define _mm256_mask_range_pd(__W, __U, __A, __B, __C) __extension__ ({       \
  (__m256d) __builtin_ia32_rangepd256_mask ((__v4df) __A, (__v4df) __B, __C, \
                (__v4df) __W, (__mmask8) __U); })

#define _mm256_maskz_range_pd(__U, __A, __B, __C) __extension__ ({           \
  (__m256d) __builtin_ia32_rangepd256_mask ((__v4df) __A, (__v4df) __B, __C, \
                (__v4df) _mm256_setzero_pd(), (__mmask8) __U); })

#define _mm_range_ps(__A, __B, __C) __extension__ ({                         \
  (__m128) __builtin_ia32_rangeps128_mask ((__v4sf) __A, (__v4sf) __B, __C,  \
                (__v4sf) _mm_setzero_ps(), (__mmask8) -1); })

#define _mm_mask_range_ps(__W, __U, __A, __B, __C) __extension__ ({          \
  (__m128) __builtin_ia32_rangeps128_mask ((__v4sf) __A, (__v4sf) __B, __C,  \
                (__v4sf) __W, (__mmask8) __U); })

#define _mm_maskz_range_ps(__U, __A, __B, __C) __extension__ ({              \
  (__m128) __builtin_ia32_rangeps128_mask ((__v4sf) __A, (__v4sf) __B, __C,  \
                (__v4sf) _mm_setzero_ps(), (__mmask8) __U); })

#define _mm256_range_ps(__A, __B, __C) __extension__ ({                      \
  (__m256) __builtin_ia32_rangeps256_mask ((__v8sf) __A, (__v8sf) __B, __C,  \
                (__v8sf) _mm256_setzero_ps(), (__mmask8) -1); })

#define _mm256_mask_range_ps(__W, __U, __A, __B, __C) __extension__ ({       \
  (__m256) __builtin_ia32_rangeps256_mask ((__v8sf) __A, (__v8sf) __B, __C,  \
                (__v8sf) __W, (__mmask8) __U); })

#define _mm256_maskz_range_ps(__U, __A, __B, __C) __extension__ ({           \
  (__m256) __builtin_ia32_rangeps256_mask ((__v8sf) __A, (__v8sf) __B, __C,  \
                (__v8sf) _mm256_setzero_ps(), (__mmask8) __U); })

#define _mm_reduce_pd(__A, __B) __extension__ ({                \
  (__m128d) __builtin_ia32_reducepd128_mask ((__v2df) __A, __B, \
                (__v2df) _mm_setzero_pd(), (__mmask8) -1); })

#define _mm_mask_reduce_pd(__W, __U, __A, __B) __extension__ ({ \
  (__m128d) __builtin_ia32_reducepd128_mask ((__v2df) __A, __B, \
                (__v2df) __W, (__mmask8) __U); })

#define _mm_maskz_reduce_pd(__U, __A, __B) __extension__ ({     \
  (__m128d) __builtin_ia32_reducepd128_mask ((__v2df) __A, __B, \
                (__v2df) _mm_setzero_pd(), (__mmask8) __U); })

#define _mm256_reduce_pd(__A, __B) __extension__ ({                \
  (__m256d) __builtin_ia32_reducepd256_mask ((__v4df) __A, __B,    \
                (__v4df) _mm256_setzero_pd(), (__mmask8) -1); })

#define _mm256_mask_reduce_pd(__W, __U, __A, __B) __extension__ ({ \
  (__m256d) __builtin_ia32_reducepd256_mask ((__v4df) __A, __B,    \
                (__v4df) __W, (__mmask8) __U); })

#define _mm256_maskz_reduce_pd(__U, __A, __B) __extension__ ({     \
  (__m256d) __builtin_ia32_reducepd256_mask ((__v4df) __A, __B,    \
                (__v4df) _mm256_setzero_pd(), (__mmask8) __U); })

#define _mm_reduce_ps(__A, __B) __extension__ ({                   \
  (__m128) __builtin_ia32_reduceps128_mask ((__v4sf) __A, __B,     \
                (__v4sf) _mm_setzero_ps(), (__mmask8) -1); })

#define _mm_mask_reduce_ps(__W, __U, __A, __B) __extension__ ({    \
  (__m128) __builtin_ia32_reduceps128_mask ((__v4sf) __A, __B,     \
                (__v4sf) __W, (__mmask8) __U); })

#define _mm_maskz_reduce_ps(__U, __A, __B) __extension__ ({        \
  (__m128) __builtin_ia32_reduceps128_mask ((__v4sf) __A, __B,     \
                (__v4sf) _mm_setzero_ps(), (__mmask8) __U); })

#define _mm256_reduce_ps(__A, __B) __extension__ ({                \
  (__m256) __builtin_ia32_reduceps256_mask ((__v8sf) __A, __B,     \
                (__v8sf) _mm256_setzero_ps(), (__mmask8) -1); })

#define _mm256_mask_reduce_ps(__W, __U, __A, __B) __extension__ ({ \
  (__m256) __builtin_ia32_reduceps256_mask ((__v8sf) __A, __B,     \
                (__v8sf) __W, (__mmask8) __U); })

#define _mm256_maskz_reduce_ps(__U, __A, __B) __extension__ ({     \
  (__m256) __builtin_ia32_reduceps256_mask ((__v8sf) __A, __B,     \
                (__v8sf) _mm256_setzero_ps(), (__mmask8) __U); })

static __inline__ __mmask8 __DEFAULT_FN_ATTRS
_mm_movepi32_mask (__m128i __A)
{
  return (__mmask8) __builtin_ia32_cvtd2mask128 ((__v4si) __A);
}

static __inline__ __mmask8 __DEFAULT_FN_ATTRS
_mm256_movepi32_mask (__m256i __A)
{
  return (__mmask8) __builtin_ia32_cvtd2mask256 ((__v8si) __A);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_movm_epi32 (__mmask8 __A)
{
  return (__m128i) __builtin_ia32_cvtmask2d128 (__A);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_movm_epi32 (__mmask8 __A)
{
  return (__m256i) __builtin_ia32_cvtmask2d256 (__A);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_movm_epi64 (__mmask8 __A)
{
  return (__m128i) __builtin_ia32_cvtmask2q128 (__A);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_movm_epi64 (__mmask8 __A)
{
  return (__m256i) __builtin_ia32_cvtmask2q256 (__A);
}

static __inline__ __mmask8 __DEFAULT_FN_ATTRS
_mm_movepi64_mask (__m128i __A)
{
  return (__mmask8) __builtin_ia32_cvtq2mask128 ((__v2di) __A);
}

static __inline__ __mmask8 __DEFAULT_FN_ATTRS
_mm256_movepi64_mask (__m256i __A)
{
  return (__mmask8) __builtin_ia32_cvtq2mask256 ((__v4di) __A);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_broadcast_f32x2 (__m128 __A)
{
  return (__m256) __builtin_ia32_broadcastf32x2_256_mask ((__v4sf) __A,
                (__v8sf)_mm256_undefined_ps(),
                (__mmask8) -1);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_mask_broadcast_f32x2 (__m256 __O, __mmask8 __M, __m128 __A)
{
  return (__m256) __builtin_ia32_broadcastf32x2_256_mask ((__v4sf) __A,
                (__v8sf) __O,
                __M);
}

static __inline__ __m256 __DEFAULT_FN_ATTRS
_mm256_maskz_broadcast_f32x2 (__mmask8 __M, __m128 __A)
{
  return (__m256) __builtin_ia32_broadcastf32x2_256_mask ((__v4sf) __A,
                (__v8sf) _mm256_setzero_ps (),
                __M);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_broadcast_f64x2 (__m128d __A)
{
  return (__m256d) __builtin_ia32_broadcastf64x2_256_mask ((__v2df) __A,
                 (__v4df)_mm256_undefined_pd(),
                 (__mmask8) -1);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_mask_broadcast_f64x2 (__m256d __O, __mmask8 __M, __m128d __A)
{
  return (__m256d) __builtin_ia32_broadcastf64x2_256_mask ((__v2df) __A,
                 (__v4df) __O, 
                 __M);
}

static __inline__ __m256d __DEFAULT_FN_ATTRS
_mm256_maskz_broadcast_f64x2 (__mmask8 __M, __m128d __A)
{
  return (__m256d) __builtin_ia32_broadcastf64x2_256_mask ((__v2df) __A,
                 (__v4df) _mm256_setzero_ps (),
                 __M);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_broadcast_i32x2 (__m128i __A)
{
  return (__m128i) __builtin_ia32_broadcasti32x2_128_mask ((__v4si) __A,
                 (__v4si)_mm_undefined_si128(),
                 (__mmask8) -1);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_mask_broadcast_i32x2 (__m128i __O, __mmask8 __M, __m128i __A)
{
  return (__m128i) __builtin_ia32_broadcasti32x2_128_mask ((__v4si) __A,
                 (__v4si) __O,
                 __M);
}

static __inline__ __m128i __DEFAULT_FN_ATTRS
_mm_maskz_broadcast_i32x2 (__mmask8 __M, __m128i __A)
{
  return (__m128i) __builtin_ia32_broadcasti32x2_128_mask ((__v4si) __A,
                 (__v4si) _mm_setzero_si128 (),
                 __M);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_broadcast_i32x2 (__m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti32x2_256_mask ((__v4si) __A,
                 (__v8si)_mm256_undefined_si256(),
                 (__mmask8) - 1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_broadcast_i32x2 (__m256i __O, __mmask8 __M, __m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti32x2_256_mask ((__v4si) __A,
                 (__v8si) __O,
                 __M);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_broadcast_i32x2 (__mmask8 __M, __m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti32x2_256_mask ((__v4si) __A,
                 (__v8si) _mm256_setzero_si256 (),
                 __M);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_broadcast_i64x2 (__m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti64x2_256_mask ((__v2di) __A,
                 (__v4di)_mm256_undefined_si256(),
                 (__mmask8) - 1);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_mask_broadcast_i64x2 (__m256i __O, __mmask8 __M, __m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti64x2_256_mask ((__v2di) __A,
                 (__v4di) __O, 
                 __M);
}

static __inline__ __m256i __DEFAULT_FN_ATTRS
_mm256_maskz_broadcast_i64x2 (__mmask8 __M, __m128i __A)
{
  return (__m256i) __builtin_ia32_broadcasti64x2_256_mask ((__v2di) __A,
                 (__v4di) _mm256_setzero_si256 (),
                 __M);
}

#define _mm256_extractf64x2_pd( __A, __imm) __extension__ ({ \
__builtin_ia32_extractf64x2_256_mask ((__v4df)( __A),\
               ( __imm),\
               (__v2df) _mm_setzero_pd (),\
               (__mmask8) -1);\
})

#define _mm256_mask_extractf64x2_pd( __W, __U, __A, __imm) __extension__ ({ \
__builtin_ia32_extractf64x2_256_mask ((__v4df)( __A),\
               ( __imm),\
               (__v2df)( __W),\
               (__mmask8) ( __U));\
})

#define _mm256_maskz_extractf64x2_pd( __U, __A, __imm) __extension__ ({ \
__builtin_ia32_extractf64x2_256_mask ((__v4df)( __A),\
               ( __imm),\
               (__v2df) _mm_setzero_pd (),\
               (__mmask8) ( __U));\
})

#define _mm256_extracti64x2_epi64( __A, __imm) __extension__ ({ \
__builtin_ia32_extracti64x2_256_mask ((__v4di)( __A),\
               ( __imm),\
               (__v2di) _mm_setzero_di (),\
               (__mmask8) -1);\
})

#define _mm256_mask_extracti64x2_epi64( __W, __U, __A, __imm) __extension__ ({ \
__builtin_ia32_extracti64x2_256_mask ((__v4di)( __A),\
               ( __imm),\
               (__v2di)( __W),\
               (__mmask8) ( __U));\
})

#define _mm256_maskz_extracti64x2_epi64( __U, __A, __imm) __extension__ ({ \
__builtin_ia32_extracti64x2_256_mask ((__v4di)( __A),\
               ( __imm),\
               (__v2di) _mm_setzero_di (),\
               (__mmask8) ( __U));\
})

#define _mm256_insertf64x2( __A, __B, __imm) __extension__ ({ \
__builtin_ia32_insertf64x2_256_mask ((__v4df)( __A),\
              (__v2df)( __B),\
              ( __imm),\
              (__v4df) _mm256_setzero_pd (),\
              (__mmask8) -1);\
})

#define _mm256_mask_insertf64x2( __W, __U, __A, __B, __imm) __extension__ ({ \
__builtin_ia32_insertf64x2_256_mask ((__v4df)( __A),\
              (__v2df)( __B),\
              ( __imm),\
              (__v4df)( __W),\
              (__mmask8) ( __U));\
})

#define _mm256_maskz_insertf64x2( __U, __A, __B, __imm) __extension__ ({ \
__builtin_ia32_insertf64x2_256_mask ((__v4df)( __A),\
              (__v2df)( __B),\
              ( __imm),\
              (__v4df) _mm256_setzero_pd (),\
              (__mmask8) ( __U));\
})

#define _mm256_inserti64x2( __A, __B, __imm) __extension__ ({ \
__builtin_ia32_inserti64x2_256_mask ((__v4di)( __A),\
              (__v2di)( __B),\
              ( __imm),\
              (__v4di) _mm256_setzero_si256 (),\
              (__mmask8) -1);\
})

#define _mm256_mask_inserti64x2( __W, __U, __A, __B, __imm) __extension__ ({ \
__builtin_ia32_inserti64x2_256_mask ((__v4di)( __A),\
              (__v2di)( __B),\
              ( __imm),\
              (__v4di)( __W),\
              (__mmask8) ( __U));\
})

#define _mm256_maskz_inserti64x2( __U, __A, __B, __imm) __extension__ ({ \
__builtin_ia32_inserti64x2_256_mask ((__v4di)( __A),\
              (__v2di)( __B),\
              ( __imm),\
              (__v4di) _mm256_setzero_si256 (),\
              (__mmask8) ( __U));\
})

#undef __DEFAULT_FN_ATTRS

#endif
