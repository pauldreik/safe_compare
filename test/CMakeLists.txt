# CMake file for the unit tests
# License:
# dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
# at your option).
# By Paul Dreik 20180923

option(VERBOSE_UNITTESTS "enables more verbose printouts during unit tests" Off)



set(source_files
   basic_test.cpp
   operations.cpp
   unittest_main.cpp
   )
      
add_executable(safe_compare_test ${source_files})

# Visual studio gets problem C1128 for debug 64 bit builds.
# see https://docs.microsoft.com/sv-se/cpp/error-messages/compiler-errors-1/fatal-error-c1128?view=vs-2017
if(MSVC)
   target_compile_options(safe_compare_test PRIVATE "/bigobj")
endif()

# We need catch for unit testing. Do we already have it?
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("catch.hpp" CATCH_FOUND)
if(CATCH_FOUND)
  # System wide install, manual intervention, or works by plain magic.
else()
  file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.4.1/catch.hpp ${CMAKE_CURRENT_SOURCE_DIR}/catch.hpp)
endif()

if(VERBOSE_UNITTESTS)
   target_compile_definitions(safe_compare_test PRIVATE SAFE_COMPARE_VERBOSE_UNITTESTS=1)
else()
   target_compile_definitions(safe_compare_test PRIVATE SAFE_COMPARE_VERBOSE_UNITTESTS=0)
endif()

# We want to carry out the tests with a native type that can hold all
# builtin integer types. on gcc and clang, there is the __int128 type
INCLUDE(CheckCXXSourceCompiles) 
check_cxx_source_compiles("int main() { __int128 x{};}" HAVE_INT128_TYPE)
if(HAVE_INT128_TYPE)
   target_compile_definitions(safe_compare_test PRIVATE HAVE_INT128_TYPE=1)
else()
   #__int28 not found. let's use boost multiprecision instead.
   # On windows, pass -DBOOST_ROOT="c:\path\to\boost\" to CMake to get this working.
   find_package(Boost)
   if(Boost_FOUND)
      target_include_directories(safe_compare_test PRIVATE ${Boost_INCLUDE_DIR})
      target_compile_definitions(safe_compare_test PRIVATE HAVE_BOOST_MULTIPRECISION=1)
    else()
      target_compile_definitions(safe_compare_test PRIVATE HAVE_BOOST_MULTIPRECISION=0)
    endif()
endif()

target_link_libraries(safe_compare_test PUBLIC safe_compare)
target_include_directories(safe_compare_test PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET safe_compare_test PROPERTY CXX_STANDARD 17)

add_test(NAME test COMMAND safe_compare_test)
