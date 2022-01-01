cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(VIE)

message(STATUS "VIE> Adding Steinberg VST SDK")
add_subdirectory(third_party/steinberg_vst_sdk)

message(STATUS "VIE> Adding Tinyobjloader")
add_subdirectory(third_party/tinyobjloader)

message(STATUS "VIE> Adding STB")
add_subdirectory(third_party/stb)

message(STATUS "VIE> Adding nuklear")
add_subdirectory(third_party/nuklear)

message(STATUS "VIE> Adding Diligent engine core")
add_subdirectory(third_party/diligent_core)

message(STATUS "VIE> Adding cycfi Q")
add_subdirectory(third_party/cycfi_q)

message(STATUS "VIE> Adding main application")
add_subdirectory(src)
