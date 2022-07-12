cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(VIE)

message("cmake compiler id: ${CMAKE_CXX_COMPILER_ID}")
message("cmake compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

message(STATUS "VIE> Adding VST interfaces")
add_subdirectory(third_party/vst)

message(STATUS "VIE> Adding cycfi Q")
add_subdirectory(third_party/cycfi_q)

message(STATUS "VIE> Adding webView 2")
add_subdirectory(third_party/webview2)

message(STATUS "VIE> Adding main application")
add_subdirectory(src)
