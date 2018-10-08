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
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/detail/operations.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/detail/wrapped_integer.hpp
)
set(header_files
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/aborting_operations.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/safe_compare.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/safe_operations.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/safe_compare/throwing_operations.hpp
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
install(FILES ${detail_header_files} DESTINATION include/safe_compare/detail)

# For creating an amalgated source. This will probably break in case the path
# contains spaces.
if(UNIX)
   add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/amalgated.hpp
                      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_amalgated.sh ${detail_header_files} ${header_files}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
                      COMMENT "generating amalgated source"
                      DEPENDS  ${detail_header_files} ${header_files})
   add_custom_target(
     amalgated ALL
     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/amalgated.hpp )
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/amalgated.hpp
           DESTINATION include/safe_compare)     
endif()



#unit tests (on by default)
option(BUILD_UNITTESTS "enables the unit tests" On)
if(BUILD_UNITTESTS)
   enable_testing()
   add_subdirectory(test/)
endif()

#performance tests (off by default, since they take a long time to build)
option(BUILD_PERFORMANCE_TESTS "enables the performance tests" Off)
if(BUILD_PERFORMANCE_TESTS)
   add_subdirectory(performance)
endif()

#examples
option(BUILD_EXAMPLES "enables building the examples" On)
if(BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()

