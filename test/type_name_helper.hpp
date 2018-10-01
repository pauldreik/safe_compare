/*
 * This is a file for compile time display of a types.
 * Based on original by Howard Hinnant and others, taken from
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/20170989#20170989
 *
 * License: CC BY-SA 3.0 https://creativecommons.org/licenses/by-sa/3.0/
 */

#pragma once

#include <string_view>

/**
 * makes the name, with the type given as a template parameter.
 * @return
 */
template<class T>
constexpr std::string_view
type_name()
{
  using namespace std;
#ifdef __clang__
  string_view p = __PRETTY_FUNCTION__;
  return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
  string_view p = __PRETTY_FUNCTION__;
#if __cplusplus < 201402
  return string_view(p.data() + 36, p.size() - 36 - 1);
#else
  return string_view(p.data() + 49, p.find(';', 49) - 49);
#endif
#elif defined(_MSC_VER)
  string_view p = __FUNCSIG__;
  return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

/**
 * creates name when one has an instance of the type.
 * @param
 * @return
 */
template<class T>
constexpr std::string_view
type_name(const T&) {
  return type_name<T>();
}
