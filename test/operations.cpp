/*
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#include <catch.hpp>

#include "safe_compare/aborting_operations.hpp"
#include "safe_compare/safe_operations.hpp"
#include "safe_compare/throwing_operations.hpp"
#include "safe_compare/unsafe_operations.hpp"

#include "testsupport.hpp"

// disable warnings from the Catch framework unsafe mix of int and bool
#pragma warning(disable : 4805)

TEST_CASE("demonstrate use with throw")
{
  // demonstrates how to use a policy for what happens in case a naive
  // comparison had done the wrong thing
  using Method = safe_compare::ThrowingCompare;
  REQUIRE_THROWS(Method::le(-1, 1U));
  REQUIRE(Method::le(-1, 1));
}

TEST_CASE("demonstrate use with abort")
{
  // Demonstrates how to use a policy for what happens in case a naive
  // comparison had done the wrong thing (useful for finding bugs during
  // testing).
  using Method = safe_compare::AbortingCompare;

  // Note that this has to be disabled until Catch supports abort, see
  // https://github.com/catchorg/Catch2/issues/853
  // If you want to prove to yourself it works, enable the row below.
#if 0
  Method::le(-1, 1U);
#endif
}

// make a macro to avoid repeating
#define IMPL(opname, naiveresult, correctresult)                               \
  do {                                                                         \
    const bool naive = (naiveresult);                                          \
    const bool correct = (correctresult);                                      \
    int result = 3; /* <--integer rather than bool, for easy troubleshooting*/ \
    if (naive != correct) {                                                    \
      REQUIRE_THROWS(result = safe_compare::ThrowingCompare::opname(a, b));    \
    } else {                                                                   \
      REQUIRE_NOTHROW(result = safe_compare::ThrowingCompare::opname(a, b));   \
      REQUIRE(result == correct);                                              \
    }                                                                          \
    /* require correct compare to be, wait for it...  correct. */              \
    REQUIRE(safe_compare::CorrectCompare::opname(a, b) == correct);            \
    /* require UnsafeCompare to be equal to naive */                           \
    REQUIRE(safe_compare::UnsafeCompare::opname(a, b) == naive);               \
  } while (0);

TEST_CASE("check throw operations")
{
  // this checks that with the throwing compare, either the correct result
  // is obtained, or an exception is thrown.

  operateOnValueCombinations([](auto a, auto b) {
    const auto a128 = safe_compare_tests::Int128_t{ a };
    const auto b128 = safe_compare_tests::Int128_t{ b };

    IMPL(lt, a < b, a128 < b128);
    IMPL(le, a <= b, a128 <= b128);
    IMPL(ge, a >= b, a128 >= b128);
    IMPL(gt, a > b, a128 > b128);
    IMPL(ne, a != b, a128 != b128);
    IMPL(eq, a == b, a128 == b128);
  });
}
#undef IMPL
