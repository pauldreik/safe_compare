/*
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */
#include <catch.hpp>

#include "safe_compare/safe_compare.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

#include "fundamental_types.hpp"
#include "static_for.hpp"
#include "testsupport.hpp"
#include "type_name_helper.hpp"

namespace {
// verbose printout of what happens
constexpr const bool verbose = SAFE_COMPARE_VERBOSE_UNITTESTS;

// column width
const auto CW = std::setw(22);
const auto left = std::left;
}

/**
 * makes sure comparisons hold, by safe converting to a huge type first.
 */
template<class HugeSignedInteger, class T, class U>
void
verifyAgainstHugeInteger(T a, U b)
{
  static_assert(std::is_integral<T>::value, "a must be a basic integer type");
  static_assert(std::is_integral<U>::value, "b must be a basic integer type");

  if constexpr (!std::numeric_limits<HugeSignedInteger>::is_specialized) {
    static_assert(std::numeric_limits<HugeSignedInteger>::min() <=
                    std::numeric_limits<T>::min(),
                  "HugeSignedInteger bit is not enough for a");
    static_assert(std::numeric_limits<HugeSignedInteger>::max() >=
                    std::numeric_limits<T>::max(),
                  "HugeSignedInteger bit is not enough for a");
    static_assert(std::numeric_limits<HugeSignedInteger>::min() <=
                    std::numeric_limits<U>::min(),
                  "HugeSignedInteger bit is not enough for b");
    static_assert(std::numeric_limits<HugeSignedInteger>::max() >=
                    std::numeric_limits<U>::max(),
                  "HugeSignedInteger bit is not enough for b");
  }
  const HugeSignedInteger A{ a };
  const HugeSignedInteger B{ b };

  // this should work mathematically correct
  const auto sa = safe_compare::make_safe(a);
  const auto sb = safe_compare::make_safe(b);

  if constexpr (verbose) {
    std::ostringstream oss;
    oss << CW << left << type_name<T>() << " compared to " << CW << left
        << type_name<U>() << "  " << CW << left << +a << "  @  " << CW << left
        << +b;
    std::cout << "Studying " << oss.str() << '\n';
  }
  CAPTURE(+a);
  CAPTURE(+b);
  REQUIRE((sa < sb) == (A < B));
  REQUIRE((sa <= sb) == (A <= B));
  REQUIRE((sa > sb) == (A > B));
  REQUIRE((sa >= sb) == (A >= B));
  REQUIRE((sa == sb) == (A == B));
  REQUIRE((sa != sb) == (A != B));
}

TEST_CASE("all combinations of fundamental integer types")
{
  // Create all combinations of types, unsigned/signed versions of those.
  // Then makes interesting values for each of the types and invokes
  // the loop body with those values. This gives a lot of combinations,
  // some of them mirrored and some of them with a and b with equal types
  // and value. That is intentional, we do not want to miss out any relevant
  // combination that may go wrong.
  operateOnValueCombinations([](auto a, auto b) {
    verifyAgainstHugeInteger<safe_compare_tests::Int128_t>(a, b);
  });
}

TEST_CASE("library demonstration")
{
  int a = -1;
  unsigned b = 1;
  REQUIRE(safe_compare::make_safe(a) < safe_compare::make_safe(b));
}

TEST_CASE("using bool should not compile")
{
  int a = -1;
  bool b = 1;
  // REQUIRE(safe_compare::make_safe(a) < safe_compare::make_safe(b));
  // ... unless forcing it to be an int.
  REQUIRE(safe_compare::make_safe(a) < safe_compare::make_safe(+b));
}
