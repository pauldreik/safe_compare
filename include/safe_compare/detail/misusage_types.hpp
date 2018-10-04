/*
 * Types to get better error messages.
 *
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#pragma once

namespace safe_compare {
inline namespace v1 {

namespace detail {
struct illegal_use_of_bool_as_integer;
struct illegal_use_of_float_as_integer;
struct illegal_use_of_double_as_integer;
struct illegal_use_of_long_double_as_integer;
}
}
}
