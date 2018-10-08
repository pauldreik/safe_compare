/*
 * This contains functions to do unsafe compare,
 * useful to switch between audit and performance in
 * a central place of the user's code base.
 *
 * By Paul Dreik 20181008
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#pragma once

#include "safe_compare/detail/operations.hpp"

namespace safe_compare {
inline namespace v1 {
/**
 * gives a type whose static methods will do exactly what naive
 * C++ does - unsafe, and performant! Only use this if you
 * know your input data is ok to compare with unsafe (for instance,
 * all your input is guaranteed to be positive).
 * There is no error checking, throwing, aborting. But it will be just
 * as fast as default C++ (after optimization).
 */
using UnsafeCompare = detail::Dispatcher<detail::UnsafeBase>;
} // namespace v1
} // namespace safe_compare
