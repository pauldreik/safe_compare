# CMake file for the examples
# License:
# dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
# at your option).
# By Paul Dreik 20181008

set(sources "01_basic_use;02_function_syntax;03_audit;")

foreach(name ${sources})
  add_executable(${name} ${name})
  target_link_libraries(${name}  PUBLIC safe_compare)
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
endforeach()

