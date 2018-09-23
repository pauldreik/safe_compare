# CMake file for the unit tests
# License:
# dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
# at your option).
# By Paul Dreik 20180923

set(source_files
   main.cpp
   test.cpp)
      
add_executable(safe_compare_test ${source_files})

#we want to carry out the tests with a native type that can hold all
#builtin integer types. on gcc and clang, there is the __int128 type
INCLUDE(CheckCXXSourceCompiles) 
check_cxx_source_compiles("int main() { __int128 x{};}" HAVE_INT128_TYPE)
if(HAVE_INT128_TYPE)
   target_compile_definitions(safe_compare_test PRIVATE HAVE_INT128_TYPE=1)
endif()
check_cxx_source_compiles("
 #include <boost/multiprecision/cpp_int.hpp> 
 int main() {boost::multiprecision::int128_t x{};}" HAVE_BOOST_MULTIPRECISION)
if(HAVE_BOOST_MULTIPRECISION)
   target_compile_definitions(safe_compare_test PRIVATE HAVE_BOOST_MULTIPRECISION=1)
endif()

target_link_libraries(safe_compare_test PUBLIC safe_compare)
target_include_directories(safe_compare_test PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET safe_compare_test PROPERTY CXX_STANDARD 17)

add_test(NAME test COMMAND safe_compare_test)
