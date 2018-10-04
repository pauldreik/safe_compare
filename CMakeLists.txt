# CMakelists for the root directory
#
# This cmake file is partially based on https://github.com/foonathan/type_safe,
# which is MIT licensed. I am uncertain what that means to the licensing
# of this file. To the largest extent possible in this situation, my changes are
# dual licensed under Boost 1.0 license or GPLv2 (or later, at your option).
#
# By Paul Dreik 20180923

cmake_minimum_required(VERSION 3.5)
project(libsafecompare VERSION 0.0.0 LANGUAGES CXX)

set(detail_header_files
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/detail/misusage_types.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/detail/wrapped_integer.hpp
)
set(header_files
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/safe_compare.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/safe_operations.hpp
)

set(target_name safe_compare)

add_library(${target_name} INTERFACE)
target_compile_features(${target_name}     INTERFACE cxx_std_14)
target_sources(${target_name}              INTERFACE $<BUILD_INTERFACE:${detail_header_files} ${header_files}>)
#target_include_directories(${target_name} INTERFACE include/)
target_include_directories(${target_name}  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(${target_name}  SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
target_compile_definitions(${target_name}  INTERFACE)

#for installation
install(FILES ${header_files}        DESTINATION include/safe_compare)
install(FILES ${detail_header_files} DESTINATION include/detail/safe_compare)

enable_testing()
add_subdirectory(test/)

