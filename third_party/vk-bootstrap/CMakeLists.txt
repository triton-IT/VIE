message(STATUS "VIE> vk-bootstrap> Fetch")

include(FetchContent)

FetchContent_Declare(
    vk-bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG        v0.4
)

FetchContent_GetProperties(vk-bootstrap)
if(NOT vk-bootstrap_POPULATED)
	FetchContent_Populate(vk-bootstrap)
endif()

option(BUILD_SHARED_LIBS "Building static library" OFF)

set(vk-bootstrap-Source
  ${vk-bootstrap_SOURCE_DIR}/src/VkBootstrap.h
  ${vk-bootstrap_SOURCE_DIR}/src/VkBootstrap.cpp
)

message(STATUS "VIE> vk-bootstrap> Add library")
add_library(vk-bootstrap STATIC ${vk-bootstrap-Source})

message(STATUS "VIE> vk-bootstrap> Using vulkan at: ${Vulkan_INCLUDE_DIR}")
target_include_directories(vk-bootstrap PUBLIC ${Vulkan_INCLUDE_DIRS})
	
message(STATUS "VIE> vk-bootstrap> Include directory: " ${vk-bootstrap_SOURCE_DIR}/src)
target_include_directories(vk-bootstrap INTERFACE ${vk-bootstrap_SOURCE_DIR}/src)

message(STATUS "VIE> vk-bootstrap> Link directory: " ${vk-bootstrap_BINARY_DIR})
target_link_directories(vk-bootstrap INTERFACE ${vk-bootstrap_BINARY_DIR})