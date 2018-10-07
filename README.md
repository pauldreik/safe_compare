# safe_compare
This is a C++14 library for safe C++ integer comparisons.

Branch | Linux/Mac | Windows
-------|-----------|--------
master | [![Build Status](https://travis-ci.com/pauldreik/safe_compare.svg?branch=master)](https://travis-ci.com/pauldreik/safe_compare) | [![Build status](https://ci.appveyor.com/api/projects/status/qfr7j9beo85y8jt5?svg=true)](https://ci.appveyor.com/project/pauldreik/safe-compare)

# Motivation
The following C++ program is not mathematically correct.
```C++
bool doit() {
    if(1U<-1) {
        return true;
    }
    return false;
}
```
Due to integer promotion rules, the -1 is converted to unsigned which (usually) means it will be a large positive number and therefore larger than 1U. This will normally give you a compiler warning so you detect it:
```
warning: comparison of integer expressions of different signedness: 'unsigned int' and 'int' [-Wsign-compare]
```
You would manually need to fix this problem which is a bit tedious. This library is here to help you with that.
```C++
#include "safe_compare/safe_compare.hpp"
bool doit() {
    if(safe_compare::make_safe(1U)<safe_compare::make_safe(-1)) {
        return true;
    }
    return false;
}
```
This is now mathematically correct. The example may look contrived, but happens quite often. A realistic example is when using int for lengths, then comparing against .size() from an STL container, like std::vector. Those use size_t, which is unsigned.

# Supported operations
The library only supports builtin integer types (by design). As of C++17, these are

  * char
  * signed char
  * unsigned char
  * wchar_t
  * char16_t
  * char32_t
  * short
  * int
  * long
  * long long

and unsigned/signed variants of those, where applicable.

Using it with some other type is a compile time error.

The supported operations are

  * ```<```
  * ```<=```
  * ```>```
  * ```>=```
  * ```==```
  * ```!=```

There are unit tests testing all combinations of types and operations listed above, on interesting values like min, max, zero, &plusmn; 1.

# Performance
There is a simple performance test included which can be run with performance/run_performance_test.sh script. It will compile the performance test at different optimization levels and type combinations, then execute it to time the results. The performance is tested on vectors of random data (the number of elements N is 10000). 
```C++
  const auto data1 = makeRandomData<T>(N);
  const auto data2 = makeRandomData<U>(N);

  std::size_t count = 0;
  bool cmp;
  for (const auto a : data1) {
    for (const auto b : data2) {
      if constexpr(Method::current==Method::plain) {
        cmp= a < b;
      } else if constexpr(Method::current==Method::correct) {
        cmp= safe_compare::CorrectCompare::lt(a,b);
      } else if constexpr(Method::current==Method::throwing) {
        cmp= safe_compare::ThrowingCompare::lt(a,b);
      }
      if (cmp) {
        ++count;
      }
    }
  }
```
The findings below come from running the benchmark with gcc 7.3 on an Intel i7-4771 CPU. 1x overhead means no overhead. 2x overhead means the same operation takes double the amount of time to complete compared to the reference.

## Runtime performance for safe comparisons on types that hadn't needed it
If comparing for instance int to long, the naive comparison is just fine. In this case, the safe_compare abstractions collapse down to no overhead already at optimization level O1 and above. That means you get compile time protection of correctness for free. This is beneficial in generic code or to proof against future refactoring.

## Runtime performance for safe comparisons on data that do not need it
If you are comparing say int32_t to uint32_t, you need safe compare. However, if you can guarantee your data to compare is always positive, the defective behaviour is not triggered so the naive comparison would be just fine. What is the price to pay for using safe_compare in that case?

At optimization level -O3, the overhead compared to naive comparison is 1-11x depending on the data type. Presumably this is because the safe compare stops certain optimizations to kick in.

The overhead of the throwing vs non-throwing safe compare is 0.5-4x depending on data type and optimization level.

## Runtime performance for safe comparisons on data that *do* need it
You can't compare the speed of a correct program to an incorrect one. If an incorrect program is allowed, we could just return constant false from the comparison and the program would be super fast.

# Building and using the library
This is a header only library. The library uses CMake. Here is how to build the unit tests on Linux.
```sh
mkdir build
cd build
cmake ..
make
```
You can also install the library, using the conventional cmake mechanism to specify the install dir. For a default system wide install, ```make install``` should do the trick.

Running the unit tests require C++17 and either a compiler that supports __int128 (like gcc and clang on x86-64) or that boost is installed, in which case boost multiprecision is used. 

# License
The TL;DR version: you can use the primary library headers commercially if you want, under the Boost 1.0 License.

The library uses several licenses. The relevant part of the library (what you would use in other projects) uses dual licensing. Pick the one you prefer:

  * Boost 1.0
  * GPLv2 (or later, at your option)

Regarding support files, some of them are based on code from other projects.

  * CMakeLists.txt are partially based on the strong types library (by Jonathan Müller), which is MIT licensed.
  * Some support files used for the unit test are based on code from Stack Overflow, so they use CC BY-SA 3.0.

Each individual file is clearly marked at the top which license it uses.
