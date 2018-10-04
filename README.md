# safe_compare
This is a C++14 library for safe C++ integer comparisons.

Branch | Linux/Mac | Windows
-------|-----------|--------
master | [![Build Status](https://travis-ci.com/pauldreik/safe_compare.svg?branch=master)](https://travis-ci.com/pauldreik/safe_compare) | [![Build status](https://ci.appveyor.com/api/projects/status/qfr7j9beo85y8jt5?svg=true)](https://ci.appveyor.com/project/pauldreik/safe-compare)

# Motivation
The following C++ program is not mathematically correct.
```
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
```
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

# Building and using the library
This is a header only library. The library uses CMake. Here is how to build the unit tests on Linux.
```
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
