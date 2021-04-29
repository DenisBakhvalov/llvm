// RUN: %clangxx -fsycl -c -fsycl-device-only -Xclang -emit-llvm %s -o - | FileCheck %s

// This test checks the codegen for the following ESIMD APIs:
// sin, cos, exp, log.

#include <CL/sycl.hpp>
#include <CL/sycl/INTEL/esimd.hpp>

using namespace cl::sycl;
using namespace sycl::ext::intel::experimental::esimd;

// Math sin,cos,log,exp functions are translated into scalar __spirv_ocl_ calls
SYCL_ESIMD_FUNCTION SYCL_EXTERNAL simd<float, 16> sw_math(simd<float, 16> x) {
  simd<float, 16> v = 0;
  //CHECK: call spir_func float @_Z15__spirv_ocl_cosf
  v = esimd_cos(x);
  //CHECK: call spir_func float @_Z15__spirv_ocl_sinf
  v = esimd_sin(v);
  //CHECK: call spir_func float @_Z15__spirv_ocl_logf
  v = esimd_log(v);
  //CHECK: call spir_func float @_Z15__spirv_ocl_expf
  v = esimd_exp(v);
  return v;
}

// Math sin,cos,log,exp functions from native namespace are translated
// into vector __esimd_ calls, which later translate into GenX intrinsics.
SYCL_ESIMD_FUNCTION SYCL_EXTERNAL simd<float, 16> native_math(simd<float, 16> x) {
  simd<float, 16> v = 0;
  namespace esimd_native_ns = sycl::ext::intel::experimental::esimd::native;
  //CHECK: call spir_func <16 x float> @_Z11__esimd_cos
  v = esimd_native_ns::esimd_cos(x);
  //CHECK: call spir_func <16 x float> @_Z11__esimd_sin
  v = esimd_native_ns::esimd_sin(v);
  //CHECK: call spir_func <16 x float> @_Z11__esimd_log
  v = esimd_native_ns::esimd_log(v);
  //CHECK: call spir_func <16 x float> @_Z11__esimd_exp
  v = esimd_native_ns::esimd_exp(v);
  return v;
}
