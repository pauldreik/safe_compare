/*
 * help functions and types to work with all the fundamental C++ types
 *
 * By Paul Dreik 2018
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */
#pragma once

#include <tuple>


struct fundamental_types {
  // see https://en.cppreference.com/w/cpp/language/types for a list
  // of distinct types.
  using fundamental_chars =
      std::tuple<signed char, unsigned char, char, wchar_t, char16_t, char32_t>;
    using fundamental_ints = std::tuple<short, int, long, long long>;

    // combine them into a large collection of types
    using all = decltype(std::tuple_cat(fundamental_chars{}, fundamental_ints{}));
};

