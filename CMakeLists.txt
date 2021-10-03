cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

#project(Vie CXX)
project(Vie)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Vie/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Vie/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Vie/bin)

set(CYCFI_Q_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cycfi/q" CACHE PATH "Cycfi q directory.")
set(CYCFI_ELEMENTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cycfi/elements/" CACHE PATH "Cycfi elements directory.")
set(STEINBERG_VST3SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../steinberg/vst3sdk-3.7.2/vst3sdk" CACHE PATH "Steinberg VST3 SDK directory.")
set(NLOHMANN_JSON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../nlohmann/json" CACHE PATH "Nlohmann json directory.")

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
#message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

#if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
#    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
#endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
#if(MSVC)
    # remove default flags provided with CMake for MSVC
#    set(CMAKE_CXX_FLAGS "")
#    set(CMAKE_CXX_FLAGS_DEBUG "")
#    set(CMAKE_CXX_FLAGS_RELEASE "")
#endif()

################################################################################
# Global linker options
################################################################################
#if(MSVC)
    # remove default flags provided with CMake for MSVC
#    set(CMAKE_EXE_LINKER_FLAGS "")
#    set(CMAKE_MODULE_LINKER_FLAGS "")
#    set(CMAKE_SHARED_LINKER_FLAGS "")
#    set(CMAKE_STATIC_LINKER_FLAGS "")
#    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
#    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
#    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
#    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
#    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
#    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
#    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
#endif()

################################################################################
# Nuget packages function stub.
################################################################################
#function(use_package TARGET PACKAGE VERSION)
#    message(WARNING "No implementation of use_package. Create yours. "
#                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
#                    "for target \"${TARGET}\" is ignored!")
#endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
#include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

#if(WIN32)
#    set(ELEMENTS_HOST_UI_LIBRARY "win32" CACHE STRING "gtk, cocoa or win32")
#elseif(UNIX AND NOT APPLE)
#    set(ELEMENTS_HOST_UI_LIBRARY "gtk" CACHE STRING "gtk, cocoa or win32")
#elseif(APPLE)
#    set(ELEMENTS_HOST_UI_LIBRARY "cocoa" CACHE STRING "gtk, cocoa or win32")
#endif()

add_subdirectory(${CYCFI_ELEMENTS_DIR}/ ${CMAKE_BINARY_DIR}/elements)
add_subdirectory(${STEINBERG_VST3SDK_DIR} ${CMAKE_BINARY_DIR}/vst_sdk)

add_subdirectory(Vie)

