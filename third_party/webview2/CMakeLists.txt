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

if (AMD64 STREQUAL ${CMAKE_SYSTEM_PROCESSOR})
    set(ARCH x64)
elseif(ARM64 STREQUAL ${CMAKE_SYSTEM_PROCESSOR})
    set(ARCH arm64)
else()
    set(ARCH x86)
endif()
message(STATUS "VIE> webview2> webview-architecture=${ARCH}")

add_library(webview_static INTERFACE)

message(STATUS "VIE> webview2> Include directories: ${webview2_SOURCE_DIR}/include/")
target_include_directories(webview_static INTERFACE ${webview2_SOURCE_DIR}/include/)

message(STATUS "VIE> webview2> Link directories: ${webview2_SOURCE_DIR}/${ARCH}/")
target_link_directories(webview_static INTERFACE ${webview2_SOURCE_DIR}/${ARCH}/)

message(STATUS "VIE> webview2> Link libraries: WebView2LoaderStatic")
target_link_libraries(webview_static INTERFACE WebView2LoaderStatic)