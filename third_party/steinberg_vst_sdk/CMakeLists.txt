message(STATUS "VIE> VST SDK> Add library")
add_library(vst_sdk INTERFACE)

message(STATUS "VIE> VST SDK> Fetch")

include(FetchContent)

# Configure Steingberg VST3 SDK
set(SMTG_ADD_VST3_PLUGINS_SAMPLES OFF CACHE BOOL "")
set(SMTG_ADD_VSTGUI OFF CACHE BOOL "")
set(SMTG_CREATE_BUNDLE_FOR_WINDOWS OFF CACHE BOOL "")
set(SMTG_CREATE_PLUGIN_LINK OFF CACHE BOOL "")
set(VSTGUI_ENABLE_DEPRECATED_METHODS OFF CACHE BOOL "")
set(SMTG_CXX_STANDARD 17 CACHE STRING "")

FetchContent_Declare(
    vst_sdk
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk
    GIT_TAG        v3.7.3_build_20
)

FetchContent_MakeAvailable(vst_sdk)

message(STATUS "VIE> VST SDK> Include directory: " ${vst_sdk_SOURCE_DIR})
target_include_directories(vst_sdk INTERFACE ${vst_sdk_SOURCE_DIR})