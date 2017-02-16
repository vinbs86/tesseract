///////////////////////////////////////////////////////////////////////
// File:        simddetect.h
// Description: Architecture detector.
// Author:      Stefan Weil (based on code from Ray Smith)
//
// (C) Copyright 2014, Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////

#include "simddetect.h"
#include "tprintf.h"

#if defined(__GNUC__)
# include <cpuid.h>
#elif defined(_WIN32)
# include <intrin.h>
#endif

SIMDDetect SIMDDetect::detector;

// If true, then AVX has been detected.
bool SIMDDetect::avx_available_;
// If true, then SSe4.1 has been detected.
bool SIMDDetect::sse_available_;

// Constructor.
// Tests the architecture in a system-dependent way to detect AVX, SSE and
// any other available SIMD equipment.
// __GNUC__ is also defined by compilers that include GNU extensions such as clang.
SIMDDetect::SIMDDetect() {
#if defined(__GNUC__)
  unsigned int eax, ebx, ecx, edx;
  if (__get_cpuid(1, &eax, &ebx, &ecx, &edx) != 0) {
#if defined(__SSE4_1__)
    sse_available_ = (ecx & 0x00080000) != 0;
#endif
#if defined(__AVX__)
    avx_available_ = (ecx & 0x10000000) != 0;
#endif
  }
#elif defined(_WIN32)
  int cpuInfo[4];
  __cpuid(cpuInfo, 0);
  if (cpuInfo[0] >= 1) {
    __cpuid(cpuInfo, 1);
#if defined(__SSE4_1__)
    sse_available_ = (cpuInfo[2] & 0x00080000) != 0;
#endif
#if defined(__AVX__)
    avx_available_ = (cpuInfo[2] & 0x10000000) != 0;
#endif
  }
#else
#  error "I don't know how to test for SIMD with this compiler"
#endif
}
