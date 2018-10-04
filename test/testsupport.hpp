/*
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */
#pragma once

#include <array>
#include <limits>
#include <type_traits>

#include "fundamental_types.hpp"
#include "static_for.hpp"

#if HAVE_INT128_TYPE
// will use native __int128 (works on clang and gcc, amd64)
#elif HAVE_BOOST_MULTIPRECISION
// use boost multiprecision
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/limits.hpp>
#else
#error "no 128 bit integer types available"
#endif

namespace safe_compare_tests {
// make a 128 bit integer type
#if HAVE_INT128_TYPE
using Int128_t = __int128;
#elif HAVE_BOOST_MULTIPRECISION
using Int128_t = boost::multiprecision::int128_t;
#else
#error "no 128 bit type available"
#endif
}

namespace testdetail {

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
template<class T, class U, class Func>
void
verifyBasicMath(Func f)
{
  // make some interesting numbers and test them pairwise
  constexpr const auto some_T = makeInteresting<T>();
  constexpr const auto some_U = makeInteresting<U>();
  for (const auto t : some_T) {
    for (const auto u : some_U) {
      f(t, u);
    }
  }
}

template<class T, class U, class Func>
void
testTypePair_inner(Func f)
{
  verifyBasicMath<T, U, Func>(f);
}

template<class T, class U, class Func>
void
testTypePair(T, U, Func f)
{
  // try out both what was given, and all combinations of adding/removing
  // signedness to the types.
  testTypePair_inner<T, U>(f);
  testTypePair_inner<std::make_unsigned_t<T>, std::make_signed_t<U>>(f);
  testTypePair_inner<std::make_unsigned_t<T>, std::make_unsigned_t<U>>(f);
  testTypePair_inner<std::make_signed_t<T>, std::make_signed_t<U>>(f);
  testTypePair_inner<std::make_signed_t<T>, std::make_unsigned_t<U>>(f);
}
}

/**
 * this will invoke f with combinations of interesting values.
 * f must be possible to invoke with f(a,b) where a and b are integers.
 * @param f
 */
template<typename Func>
void
operateOnValueCombinations(Func f)
{
  static_foreach(fundamental_types::all{}, [&](std::size_t i, auto dummy1) {
    static_foreach(fundamental_types::all{}, [&](std::size_t j, auto dummy2) {
      testdetail::testTypePair(dummy1, dummy2, f);
    });
  });
}
