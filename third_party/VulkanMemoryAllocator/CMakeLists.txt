message(STATUS "VIE> VMA> Fetch")

include(FetchContent)

FetchContent_Declare(
    vk_mem_alloc
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
    GIT_TAG        v2.3.0+vs2017
)

FetchContent_GetProperties(vk_mem_alloc)
if(NOT vk_mem_alloc_POPULATED)
	FetchContent_Populate(vk_mem_alloc)
	
	message(STATUS "VIE> VMA> Add library")
	add_library(vk_mem_alloc INTERFACE)
endif()

message(STATUS "VIE> vma> Include directory: " ${vk_mem_alloc_SOURCE_DIR}/src)
target_include_directories(vk_mem_alloc INTERFACE ${vk_mem_alloc_SOURCE_DIR}/src)