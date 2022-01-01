message(STATUS "VIE> Nuklear> Fetch")

include(FetchContent)

FetchContent_Declare(
    nuklear
    GIT_REPOSITORY https://github.com/Immediate-Mode-UI/Nuklear
    GIT_TAG        4.9.5
)

FetchContent_GetProperties(nuklear)
if(NOT nuklear_POPULATED)
	FetchContent_Populate(nuklear)
endif()

message(STATUS "VIE> Nuklear> Add library")
add_library(nuklear INTERFACE)

message(STATUS "VIE> Nuklear> Include directory: " ${nuklear_SOURCE_DIR})
target_include_directories(nuklear INTERFACE ${nuklear_SOURCE_DIR})
