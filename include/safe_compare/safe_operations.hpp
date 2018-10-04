/*
 * This contains functions to do safe compare
 *
 * By Paul Dreik 20181003
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
 * gives a type whose static methods will give the correct answer, without
 * any error reporting at all.
 */
using CorrectCompare = detail::Dispatcher<detail::NullPolicy>;
} // namespace v1
} // namespace safe_compare
