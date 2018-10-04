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

#include <cstdlib>
#include <iostream>

namespace safe_compare {
inline namespace v1 {
namespace detail {

struct AbortingPolicy
{
  template<typename Tag, typename I1, typename I2>
  static void report(Tag, I1 a, I2 b, bool naive, bool correct)
  {
    std::cerr << "safe_compare::AbortingPolicy: Naive comparison "
              << Tag::getName() << " gives the wrong result " << naive
              << ", which differs from the correct result " << correct
              << " for values " << +a << " and " << +b << std::endl;
    std::abort();
  }
};
} // namespace detail

/**
 * gives a type whose static methods will abort in case a naive comparison
 * had given the incorrect answer. Useful for smoking out bugs during testing.
 */
using AbortingCompare = detail::Dispatcher<detail::AbortingPolicy>;
} // namespace v1
} // namespace safe_compare
