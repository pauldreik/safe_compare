/*
 * for testing performance overhead
 *
 * By Paul Dreik 20181006
 *
 * License:
 * dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
 * at your option).
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>

#include "safe_compare/safe_operations.hpp"
#include "safe_compare/throwing_operations.hpp"

// make random data in range [0, max] - we do not want the throwing compare
// to actually throw, since it is the fast path that is of interest and
// benchmarking detecting an error vs not seeing it is irrelevant - correctness
// first!

template<typename Int>
std::vector<Int>
makeRandomData(std::size_t N)
{
  std::vector<Int> data;
  data.reserve(N);
  std::random_device rd{};
  std::mt19937 gen(rd());
  // Visual Studio
#if _MSC_VER
  // Visual studio errors out with not accepting char, signed char, unsigned
  // char, int8_t, and uint8_t for uniform_int_distribution
  using Promoted = decltype(+Int{});
  std::uniform_int_distribution<Promoted> dis(
    Promoted{}, static_cast<Promoted>(std::numeric_limits<Int>::max()));
  std::generate_n(
    std::back_inserter(data), N, [&]() { return static_cast<Int>(dis(gen)); });
#else
  // c++ compliant variants (see http://eel.is/c++draft/rand.dist.uni.int)
  std::uniform_int_distribution<Int> dis(Int{},
                                         std::numeric_limits<Int>::max());

  std::generate_n(std::back_inserter(data), N, [&]() { return dis(gen); });
#endif

  return data;
}
/*
 * g++ performance.cpp -I/home/paul/usr/local/include/ -O1 && time ./a.out
 *   when comparing char to int
 *                method
 *  build        c++     correct       throwing
 *  -O0                             6,975
 *  -Og                             0,725
 *  -O1                      0,227       0,143
 *  -O2                             0,220
 *  -O3         0,103                    0,107
 */

#if !defined(METHOD)
#error "please define METHOD to one of plain, correct or throwing"
#endif

enum class Method
{
  plain,
  correct,
  throwing,
  current = METHOD, // <--from preprocessor macro
};

template<typename T, typename U, std::size_t N>
std::size_t
countRandomPairs()
{
  // make "realistic" test data.
  const auto data1 = makeRandomData<T>(N);
  const auto data2 = makeRandomData<U>(N);

  std::size_t count = 0;
  bool cmp;
  for (const auto a : data1) {
    for (const auto b : data2) {
      if constexpr (Method::current == Method::plain) {
        cmp = a < b;
      } else if constexpr (Method::current == Method::correct) {
        cmp = safe_compare::CorrectCompare::lt(a, b);
      } else if constexpr (Method::current == Method::throwing) {
        cmp = safe_compare::ThrowingCompare::lt(a, b);
      }
      if (cmp) {
        ++count;
      }
    }
  }

  return count;
}
#if !defined(TYPEA)
#error "please define TYPEA to an integer type like std::int32_t or so"
#endif
#if !defined(TYPEB)
#error "please define TYPEB to an integer type like std::int32_t or so"
#endif

int
main()
{
  const std::size_t N = 10000;
  std::cout << "count for " << STRTYPEA << '<' << STRTYPEB << " :"
            << countRandomPairs<TYPEA, TYPEB, N>() << '\n';
}
