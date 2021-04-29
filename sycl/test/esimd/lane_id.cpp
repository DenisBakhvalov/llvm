// RUN: %clangxx -fsycl -c -fsycl-device-only -Xclang -emit-llvm %s -o - | FileCheck %s

// This test checks the codegen for the basic usage of SIMT_BEGIN - SIMT_END construct.

#include <CL/sycl.hpp>
#include <CL/sycl/INTEL/esimd.hpp>

using namespace cl::sycl;
using namespace sycl::INTEL::gpu;

//CHECK-LABEL: define dso_local spir_func void @_Z3fooi
//CHECK:  call spir_func void @"_ZZ3fooiENK3$_0clEv"(
SYCL_ESIMD_FUNCTION SYCL_EXTERNAL simd<int, 16> foo(int x) {
  simd<int, 16> v = 0;
  using namespace sycl::INTEL::gpu::detail;
  SIMT_BEGIN(16, lane)
//CHECK: define internal spir_func void @"_ZZ3fooiENK3$_0clEv"({{.*}}) {{.*}} #[[ATTR:[0-9]+]]
//CHECK: %{{[0-9a-zA-Z_.]+}} = tail call spir_func i32 @_Z15__esimd_lane_idv()  
    v.select<1, 0>(lane) = x++;
  SIMT_END
  return v;
}

//CHECK: attributes #[[ATTR]] = { {{.*}} "CMGenxSIMT"="16" {{.*}}}
