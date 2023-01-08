message(STATUS "VIE> wil> Fetch")

include(FetchContent)

FetchContent_Declare(
    wil
    GIT_REPOSITORY https://github.com/microsoft/wil
    GIT_TAG        fd91a817faeec8dee12558b40a6ee54a863d24a7
)

FetchContent_GetProperties(wil)
if(NOT wil_POPULATED)
	FetchContent_Populate(wil)
endif()

add_library(wil INTERFACE)

message(STATUS "VIE> wil> Include directories: ${wil_SOURCE_DIR}/include/")
target_include_directories(wil INTERFACE ${wil_SOURCE_DIR}/include/)