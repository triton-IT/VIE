message(STATUS "VIE> STB> Fetch")

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb
    GIT_TAG        af1a5bc352164740c1cc1354942b1c6b72eacb8a
)

FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
	FetchContent_Populate(stb)
endif()

message(STATUS "VIE> STB> Add library")
add_library(stb INTERFACE)

message(STATUS "VIE> STB> Include directory: " ${stb_SOURCE_DIR})
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
