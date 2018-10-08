/*
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#include "safe_compare/safe_compare.hpp"
#include <iostream>

int
main()
{
  const int a = -1;
  const unsigned b = 1;

  std::cout << "C++ default comparison:  " << a << "<" << b << " is "
            << (a < b ? "true" : "false") << '\n';

  std::cout << "safe_compare comparison: " << a << "<" << b << " is "
            << (safe_compare::make_safe(a) < safe_compare::make_safe(b)
                  ? "true"
                  : "false")
            << '\n';
}
