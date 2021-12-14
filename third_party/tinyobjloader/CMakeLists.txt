message(STATUS "VIE> tinyobjloader> Fetch")

include(FetchContent)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
    GIT_TAG        v2.0.0rc9
)

FetchContent_GetProperties(tinyobjloader)
if(NOT tinyobjloader_POPULATED)
	FetchContent_Populate(tinyobjloader)
endif()

option(BUILD_SHARED_LIBS "Building static library" OFF)

set(tinyobjloader-Source
  ${tinyobjloader_SOURCE_DIR}/tiny_obj_loader.h
  ${tinyobjloader_SOURCE_DIR}/tiny_obj_loader.cc
)

message(STATUS "VIE> tinyobjloader> Add library")
add_library(tinyobjloader STATIC ${tinyobjloader-Source})
	
message(STATUS "VIE> tinyobjloader> Include directory: " ${tinyobjloader_SOURCE_DIR})
target_include_directories(tinyobjloader INTERFACE ${tinyobjloader_SOURCE_DIR})

message(STATUS "VIE> tinyobjloader> Link directory: " ${tinyobjloader_BINARY_DIR})
target_link_directories(tinyobjloader INTERFACE ${tinyobjloader_BINARY_DIR})