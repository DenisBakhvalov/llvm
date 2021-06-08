// RUN: %clangxx -fsycl -fsyntax-only -Xclang -verify %s
// expected-no-diagnostics

#include <sycl/ext/intel/experimental/esimd.hpp>

using namespace sycl::ext::intel::experimental::esimd;

void test_simd_subscript_basic() SYCL_ESIMD_FUNCTION {
  simd<int, 4> v = 1;
  int val1 = v[0];
  v[1] = 0; // returns simd_view

  const simd<int, 4> cv = 1;
  int val2 = cv[0];
  // v[1] = 0; // error
}

void test_simd_subscript_binop() SYCL_ESIMD_FUNCTION {
  simd<int, 4> v1 = 1;
  //simd<int, 4> v2 = 2;
  // uses simd implicit conversion if there is only one element in the vector
  int val = v1[0] + v1[1];
}