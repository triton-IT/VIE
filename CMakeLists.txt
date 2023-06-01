cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "VIE> Adding VST interfaces")
add_subdirectory(third_party/vst)

message(STATUS "VIE> Adding cycfi Q")
add_subdirectory(third_party/cycfi_q)

if (WIN32)
	message(STATUS "VIE> Adding webView 2")
	add_subdirectory(third_party/webview2)

	message(STATUS "VIE> Adding wil")
	add_subdirectory(third_party/wil)
endif ()

message(STATUS "VIE> Adding libsndfile")
add_subdirectory(third_party/libsndfile)

message(STATUS "VIE> Adding main application")
add_subdirectory(src)

# TODO: Make tests compile on MACOSs
if (WIN32)
	message(STATUS "VIE> Adding catch2")
	add_subdirectory(third_party/catch2)

	message(STATUS "VIE> Adding tests application")
	add_subdirectory(tests)
endif ()