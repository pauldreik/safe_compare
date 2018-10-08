/*
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#include "safe_compare/aborting_operations.hpp"
#include "safe_compare/unsafe_operations.hpp"
#include <iostream>

bool
original_code(int a, unsigned int b)
{
  return a < b;
}

template<class SafePolicy>
bool
instrumented_mode(int a, unsigned int b)
{
  // just replace < with lt in a way that let's us easily switch
  // between modes in a central location. Here, we pass it as a
  // template parameter but a real application would put it in some central
  // header, like "using ProjectPolicy=UnsafeCompare;" or so.
  return SafePolicy::lt(a, b);
}

int
main()
{

  using namespace safe_compare;

  // this is to demonstrate an example application, where one wants to verify
  // that all naive comparisons are correct, because the input data is
  // sanitized before doing the compare.

  const int a = -1;
  const int b = 1;

  // step 1 - the original application.
  std::cout << "Result from original application: " << original_code(a, b)
            << '\n';

  // step 2 - replace it with the aborting version.
  // in a more realistic scenario, a project wide using
  // SafePolicy=AbortingCompare could have been used for the policy, instead of
  // rewriting all your code into templates.
  std::cout << "Result from application in audit mode: "
            << instrumented_mode<AbortingCompare>(a, b) << '\n';

  // we never come here unless you change the a and b values from the default,
  // since abort() was called on the line above. Run the example program in
  // a debugger if you want to see where the library threw.

  std::cout << "Your input data a and b must have been changed to good values, "
               "since we reached here.\n";
  std::cout << "This convinces me that all input data is always good, so let's "
               "skip the overhead and switch to naive comparison, for "
               "performance reasons.\n";
  std::cout << "Result for unsafe, performant mode, is: "
            << instrumented_mode<UnsafeCompare>(a, b) << '\n';

  // well, after a while it was decided it is better to be safe than sorry,
  // so we decide to throw an exception in case unexpected input data comes
  // along.
  std::cout << "Result for throwing mode is: "
            << instrumented_mode<UnsafeCompare>(a, b) << '\n';
}
