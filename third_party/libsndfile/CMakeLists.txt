message(STATUS "VIE> libsndfile> Fetch")

set(CMAKE_BUILD_TYPE RELEASE)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)
set(INSTALL_MANPAGES OFF)

include(FetchContent)

FetchContent_Declare(
        libsndfile
        GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
        GIT_TAG        1.1.0
)

FetchContent_MakeAvailable(libsndfile)

message(STATUS "VIE> libsndfile> Add library")
add_library(libsndfile INTERFACE)

message(STATUS "VIE> libsndfile> Include directories: ${libsndfile_BINARY_DIR}/include")
target_include_directories(libsndfile INTERFACE ${libsndfile_BINARY_DIR}/include)

message(STATUS "VIE> libsndfile> Link directory: " ${libsndfile_BINARY_DIR})
target_link_directories(libsndfile INTERFACE ${libsndfile_BINARY_DIR})