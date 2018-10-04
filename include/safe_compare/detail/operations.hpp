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

#include "safe_compare/detail/wrapped_integer.hpp"

namespace safe_compare {
inline namespace v1 {

namespace detail {

/**
 * for making it easy to do tag dispatch, and at the same time get a readable
 * name of the comparison.
 */
template<char ch1, char ch2 = '\0'>
struct CmpTag
{
  static const char* getName()
  {
    static const char name[] = { ch1, ch2, '\0' };
    return name;
  }
};
using EQ = CmpTag<'=', '='>;
using NE = CmpTag<'!', '='>;
using GT = CmpTag<'>'>;
using GE = CmpTag<'>', '='>;
using LT = CmpTag<'<'>;
using LE = CmpTag<'<', '='>;

template<typename I1, typename I2>
struct Correct
{
  inline static bool cmp(EQ, I1 a, I2 b)
  {
    return Value<I1>{ a } == Value<I2>{ b };
  }
  inline static bool cmp(NE, I1 a, I2 b)
  {
    return Value<I1>{ a } != Value<I2>{ b };
  }
  inline static bool cmp(GT, I1 a, I2 b)
  {
    return Value<I1>{ a } > Value<I2>{ b };
  }
  inline static bool cmp(GE, I1 a, I2 b)
  {
    return Value<I1>{ a } >= Value<I2>{ b };
  }
  inline static bool cmp(LT, I1 a, I2 b)
  {
    return Value<I1>{ a } < Value<I2>{ b };
  }
  inline static bool cmp(LE, I1 a, I2 b)
  {
    return Value<I1>{ a } <= Value<I2>{ b };
  }
}; // struct correct

// MSVC disable warnings for signed/unsigned
#pragma warning(push)
#pragma warning(disable : 4018)

template<typename I1, typename I2>
struct Naive
{
  inline static bool cmp(EQ, I1 a, I2 b) { return a == b; }
  inline static bool cmp(NE, I1 a, I2 b) { return a != b; }
  inline static bool cmp(GT, I1 a, I2 b) { return a > b; }
  inline static bool cmp(GE, I1 a, I2 b) { return a >= b; }
  inline static bool cmp(LT, I1 a, I2 b) { return a < b; }
  inline static bool cmp(LE, I1 a, I2 b) { return a <= b; }
}; // struct naive

#pragma warning(pop)

struct NullPolicy
{
  template<typename Tag, typename I1, typename I2>
  inline static void report(Tag, I1 a, I2 b, bool naive, bool correct)
  {}
};

template<typename Policy>
struct Dispatcher
{
  template<typename I1, typename I2>
  static bool eq(I1 a, I2 b)
  {
    return cmp(EQ{}, a, b);
  }
  template<typename I1, typename I2>
  static bool ne(I1 a, I2 b)
  {
    return cmp(NE{}, a, b);
  }
  template<typename I1, typename I2>
  static bool gt(I1 a, I2 b)
  {
    return cmp(GT{}, a, b);
  }
  template<typename I1, typename I2>
  static bool ge(I1 a, I2 b)
  {
    return cmp(GE{}, a, b);
  }
  template<typename I1, typename I2>
  static bool lt(I1 a, I2 b)
  {
    return cmp(LT{}, a, b);
  }
  template<typename I1, typename I2>
  static bool le(I1 a, I2 b)
  {
    return cmp(LE{}, a, b);
  }

private:
  // overload when types are equal - no need to do anything.
  template<typename I1, /* typename I2, */ typename Tag>
  inline static bool cmp(Tag, I1 a, I1 b)
  {
    return Naive<I1, I1>::cmp(Tag{}, a, b);
  }

  template<typename I1, typename I2, typename Tag>
  inline static bool cmp(Tag, I1 a, I2 b)
  {
    const bool naive = Naive<I1, I2>::cmp(Tag{}, a, b);
    const bool correct = Correct<I1, I2>::cmp(Tag{}, a, b);
    if (naive != correct) {
      Policy::report(Tag{}, a, b, naive, correct);
    }
    return correct;
  }
}; // struct Dispatcher

} // detail

} // namespace v1
} // namespace safe_compare
