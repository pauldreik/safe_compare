/*
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */
#include <catch/catch.hpp>

#include "safe_compare/safe_compare.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "fundamental_types.hpp"
#include "static_for.hpp"
#include "type_name_helper.hpp"

#if HAVE_INT128_TYPE
// will use native __int128 (works on clang and gcc, amd64)
#elif HAVE_BOOST_MULTIPRECISION
// use boost multiprecision
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/limits.hpp>
#else
#error "no 128 bit integer types available"
#endif

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
}

/**
 * makes an interesting array of values, to trigger problematic situations
 * @return
 */
template<class T>
constexpr std::array<const T, 5>
makeInteresting()
{
  constexpr const T MinusOneIfPossible =
    std::numeric_limits<T>::is_signed ? T{} - 1 : T{};
  return { std::numeric_limits<T>::min(),
           MinusOneIfPossible,
           0,
           1,
           std::numeric_limits<T>::max() };
}

/**
 * for two integral types T and U, verify comparing them works as intended.
 * Internally T and U are also tested with their types swapped, so
 * there is no need to invoke this a second time with the template arguments
 * swapped.
 */
template<class T, class U>
void
verifyBasicMath()
{
  static_assert(std::is_integral<T>::value, "a must be a basic integer type");
  static_assert(std::is_integral<U>::value, "b must be a basic integer type");

  // make a 128 bit integer type
#if HAVE_INT128_TYPE
  using Int128_t = __int128;
#elif HAVE_BOOST_MULTIPRECISION
  using Int128_t = boost::multiprecision::int128_t;
#else
#error "no 128 bit type available"
#endif

  if constexpr (verbose) {
    std::ostringstream oss;
    oss << CW << type_name<T>() << " compared to " << CW << type_name<U>()
        << '\n';
    INFO("Studying " << oss.str());
  }

  // make some interesting numbers and test them pairwise
  constexpr const auto some_T = makeInteresting<T>();
  constexpr const auto some_U = makeInteresting<U>();
  for (const auto t : some_T) {
    for (const auto u : some_U) {
      verifyAgainstHugeInteger<Int128_t>(t, u);
    }
  }
}

template<class T, class U>
void
testTypePair_inner()
{
  verifyBasicMath<T, U>();
}

template<class T, class U>
void testTypePair(T, U)
{
  // try out both what was given, and all combinations of adding/removing
  // signedness to the types.
  testTypePair_inner<T, U>();
  testTypePair_inner<std::make_unsigned_t<T>, std::make_signed_t<U>>();
  testTypePair_inner<std::make_unsigned_t<T>, std::make_unsigned_t<U>>();
  testTypePair_inner<std::make_signed_t<T>, std::make_signed_t<U>>();
  testTypePair_inner<std::make_signed_t<T>, std::make_unsigned_t<U>>();
}

TEST_CASE("all combinations of fundamental integer types")
{
  // create all combinations of types (unsigned/signed versions of each will be
  // created internally later on)
  static_foreach(fundamental_types::all{}, [&](std::size_t i, auto dummy1) {
    static_foreach(fundamental_types::all{}, [&](std::size_t j, auto dummy2) {
      testTypePair(dummy1, dummy2);
    });
  });
}

TEST_CASE("library demonstration")
{
  int a = -1;
  unsigned b = 1;
  REQUIRE(safe_compare::make_safe(a) < safe_compare::make_safe(b));
}
