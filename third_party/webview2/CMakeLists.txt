message(STATUS "VIE> webview2> Fetch")

include(FetchContent)

FetchContent_Declare(
    webview2
    GIT_REPOSITORY https://github.com/tobq/webview2-cmake
    GIT_TAG        91711058adc76849ec54728ca80c7d89234f3e48
)

FetchContent_GetProperties(webview2)
if(NOT webview2_POPULATED)
	FetchContent_Populate(webview2)
endif()

message(STATUS "VIE> webview2> webview-architecture=x64")

add_library(webview2 INTERFACE)

message(STATUS "VIE> webview2> Include directories: ${webview2_SOURCE_DIR}/include/")
target_include_directories(webview2 INTERFACE ${webview2_SOURCE_DIR}/include/)

if (CMAKE_SIZE_OF_VOID_P EQUAL 4)
    message(STATUS "VIE> webview2> Link directories: ${webview2_SOURCE_DIR}/x86/")
    target_link_directories(webview2 INTERFACE ${webview2_SOURCE_DIR}/x86/)
endif()

if (CMAKE_SIZE_OF_VOID_P EQUAL 8)
    message(STATUS "VIE> webview2> Link directories: ${webview2_SOURCE_DIR}/x64/")
    target_link_directories(webview2 INTERFACE ${webview2_SOURCE_DIR}/x64/)
endif()

message(STATUS "VIE> webview2> Link libraries: WebView2LoaderStatic")
target_link_libraries(webview2 INTERFACE WebView2LoaderStatic)