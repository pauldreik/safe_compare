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

  // this is one way of doing the compare:
  const bool naive_cpp = a < b;

  // this is by using safe compare, with the "wrap" approach:
  const bool wrap = safe_compare::make_safe(a) < safe_compare::make_safe(b);

  // this is safe compare with function approach (lt - "less than")
  const bool function_style = safe_compare::CorrectCompare::lt(a, b);

  // the CorrectCompare "function" gives the correct answer no matter the input,
  // just like the "wrap" approach.
  assert(wrap == function_style);

  // if we want to, we can make the comparison throw an exception in case
  // the naive result had differed. This will (after optimization) only give
  // overhead in case there actually is a possibility that the naive comparison
  // is incorrect, like here when signed is compared to unsigned.
  try {
    const bool throwing_result = safe_compare::ThrowingCompare::lt(a, b);
  } catch (std::exception& e) {
    std::cout << "data is such, that an exception was thrown. message: "
              << e.what() << '\n';
  }
}
