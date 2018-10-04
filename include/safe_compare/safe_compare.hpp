/*
 * License: dual license, pick your choice. Either Boost license 1.0, or GPL(v2
 * or later, at your option).
 * by Paul Dreik 20180919
 */
#pragma once

#include "detail/misusage_types.hpp"
#include "safe_compare/detail/wrapped_integer.hpp"

namespace safe_compare {

inline namespace v1 {

/**
 * converts an integer into a wrapped integer, returning an object
 * that when compared to another wrapped integer will do the mathematically
 * correct thing when compared. There will be no throwing or asserting, just
 * the correct comparison.
 * @param x
 * @return
 */
template<class Integer>
Value<Integer>
make_safe(Integer x)
{
  return Value<Integer>{ x };
}

detail::illegal_use_of_bool_as_integer
make_safe(bool) = delete;

safe_compare::detail::illegal_use_of_float_as_integer
make_safe(float) = delete;

safe_compare::detail::illegal_use_of_double_as_integer
make_safe(double) = delete;

safe_compare::detail::illegal_use_of_long_double_as_integer
make_safe(long double) = delete;
} // namespace v1
} // namespace safe_compare
