# CMake file for the examples
# License:
# dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
# at your option).
# By Paul Dreik 20180923


set(source_files
   performance.cpp
   )

#make several binaries which use different strategies
foreach(METHOD plain;correct;throwing)
#foreach(TYPEA uint8;uint16;uint32;uint64;int8;int16;int32;int64)
foreach(TYPEA uint8;uint32;int8;int32)
#foreach(TYPEB uint8;uint16;uint32;uint64;int8;int16;int32;int64)
foreach(TYPEB uint8;uint32;int8;int32)
  set(name performance_${METHOD}_${TYPEA}_${TYPEB})
  add_executable(${name} ${source_files})
  target_compile_definitions(${name}  PRIVATE METHOD=${METHOD} STRMETHOD=\"${METHOD}\")
  target_compile_definitions(${name}  PRIVATE TYPEA=std::${TYPEA}_t STRTYPEA=\"${TYPEA}\")
  target_compile_definitions(${name}  PRIVATE TYPEB=std::${TYPEB}_t STRTYPEB=\"${TYPEB}\")
  target_link_libraries(${name} PUBLIC safe_compare)
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)
endforeach()
endforeach()
endforeach()
