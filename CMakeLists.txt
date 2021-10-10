cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

project(Vie)

set(CYCFI_Q_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cycfi/q" CACHE PATH "Cycfi q directory.")
set(CYCFI_ELEMENTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cycfi/elements/" CACHE PATH "Cycfi elements directory.")
set(STEINBERG_VST3SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/steinberg/vst3sdk-3.7.2/vst3sdk" CACHE PATH "Steinberg VST3 SDK directory.")
set(NLOHMANN_JSON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/json" CACHE PATH "Nlohmann json directory.")

# Set target arch type if empty. Visual studio solution generator provides it.
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()

# Configure elements
if( WIN32)
	set(ELEMENTS_HOST_UI_LIBRARY "win32")
elseif( APPLE )
	set(ELEMENTS_HOST_UI_LIBRARY "cocoa")
elseif( UNIX )
	set(ELEMENTS_HOST_UI_LIBRARY "gtk")
endif()

set(ELEMENTS_BUILD_EXAMPLES OFF)

add_subdirectory(${CYCFI_ELEMENTS_DIR}/ ${CMAKE_BINARY_DIR}/elements)

# Configure Steingberg VST3 SDK
set(SMTG_ADD_VST3_PLUGINS_SAMPLES OFF CACHE BOOL "")
set(SMTG_ADD_VSTGUI OFF CACHE BOOL "")
set(SMTG_CREATE_BUNDLE_FOR_WINDOWS OFF CACHE BOOL "")
set(SMTG_CREATE_PLUGIN_LINK OFF CACHE BOOL "")
set(VSTGUI_ENABLE_DEPRECATED_METHODS OFF CACHE BOOL "")
set(SMTG_CXX_STANDARD 17 CACHE STRING "")

add_subdirectory(${STEINBERG_VST3SDK_DIR} ${CMAKE_BINARY_DIR}/vst_sdk)

# Configure VIE
add_subdirectory(Vie)
