message(STATUS "VIE> vie core> Fetch")

include(FetchContent)

FetchContent_Declare(
        vie_core
        GIT_REPOSITORY https://github.com/triton-IT/vie-core.git
        GIT_TAG        1bc4d75cfc6720de77239cba67045910499f784c
)

FetchContent_MakeAvailable(vie_core)

message(STATUS "VIE> vie core> Add library")
add_library(vie_core INTERFACE)

message(STATUS "VIE> vie core> Include directories: ${vie_core_BINARY_DIR}/src")
target_include_directories(vie_core INTERFACE ${vie_core_BINARY_DIR}/src)

#TODO: There is probably a way to export include directory to vie without using a variable?
set_target_properties(vie_core PROPERTIES VIE_CORE_INCLUDE_DIRECTORIES ${vie_core_SOURCE_DIR}/include)

message(STATUS "VIE> vie core> Link directory: " ${vie_core_BINARY_DIR})
target_link_directories(vie_core INTERFACE ${vie_core_BINARY_DIR})