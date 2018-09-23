/*
 * License: dual license, pick your choice. Either Boost license 1.0, or GPL(v2
 * or later, at your option).
 * by Paul Dreik 20180919
*/
#pragma once
#include <cassert>
#include <limits>
#include <type_traits>

namespace safe_compare {

template<class Integer,
         std::enable_if_t<std::is_integral<Integer>::value, Integer> = 0>
struct Value
{
  enum properties
  {
    is_signed = std::numeric_limits<Integer>::is_signed
  };
  Integer m_value;
};

namespace detail {
template<class Integer>
std::make_unsigned_t<Integer>
make_unsigned(Integer x)
{
  assert(x >= 0 && "internal programming error - call only on positive values");
  return x;
}
}

template<class Integer1, class Integer2>
bool
operator<(const Value<Integer1>& left, const Value<Integer2>& right)
{
  if (+Value<Integer1>::is_signed == +Value<Integer2>::is_signed) {
    // both are signed, or both are unsigned. c++ default rules will work
    // fine here.
    return left.m_value < right.m_value;
  }

  if (Value<Integer1>::is_signed) {
    // left is signed - right is unsigned. careful check needed.
    return left.m_value < 0 ||
           detail::make_unsigned(left.m_value) < right.m_value;
  } else {
    // left is unsigned - right is signed. careful check needed.
    return right.m_value >= 0 &&
           left.m_value < detail::make_unsigned(right.m_value);
  }
}

/**
 * converts an integer into a wrapped integer
 * @param x
 * @return
 */
template<class Integer>
Value<Integer>
make_safe(Integer x)
{
  return Value<Integer>{ x };
}
} //namespace safe_compare
