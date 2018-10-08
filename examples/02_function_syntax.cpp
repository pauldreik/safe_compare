/*
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#include "safe_compare/safe_compare.hpp"
#include "safe_compare/safe_operations.hpp"
#include "safe_compare/throwing_operations.hpp"
#include <cassert>
#include <iostream>
int
main()
{
  const int a = -1;
  const unsigned b = 1;

  // This is the default C++ way of doing the compare:
  const bool naive_cpp = a < b;

  // This is by using safe compare, with the "wrap" approach. It will give
  // the correct result.
  const bool wrap = safe_compare::make_safe(a) < safe_compare::make_safe(b);

  // This is safe compare with an alternative syntax, using a function instead.
  // lt - "less than")
  const bool function_style = safe_compare::CorrectCompare::lt(a, b);

  // The CorrectCompare "function" gives the correct answer no matter the input,
  // just like the "wrap" approach. Let's verify it.
  assert(wrap == function_style);

  // If we want to, we can make the comparison throw an exception in case
  // the naive result had differed. This will (after optimization) only give
  // overhead in case there actually is a possibility that the naive comparison
  // is incorrect, like here when signed is compared to unsigned.
  // The throwing mode is useful when you want to audit your application for
  // places where naive comparison gives the wrong result - perhaps data is
  // sanitized elsewhere, so you do not need to pay the overhead for either
  // CorrectCompare or throwing compare. This can help you verify that
  // assumption on test data, and then you switch to naive compare once you
  // convinced yourself.
  try {
    const bool throwing_result = safe_compare::ThrowingCompare::lt(a, b);
  } catch (std::exception& e) {
    std::cout << "data is such, that an exception was thrown. message: "
              << e.what() << '\n';
  }
}
