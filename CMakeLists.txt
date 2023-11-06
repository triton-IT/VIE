cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "" FORCE)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "VIE> Adding VST interfaces")
add_subdirectory(third_party/vst)

message(STATUS "VIE> Adding vie core")
add_subdirectory(third_party/vie_core)

message(STATUS "VIE> Adding webView 2")
add_subdirectory(third_party/webview2)

message(STATUS "VIE> Adding wil")
add_subdirectory(third_party/wil)

message(STATUS "VIE> Adding main application")
set(PROJECT_NAME VIE)

project(${PROJECT_NAME})

set(JSON_DIRECTORY
        "third_party/nlohman_json"
        )

# Source groups
set(HEADER_FILES
        "src/application.h"
        "src/editor_view.h"
        "src/core_logger_impl.h"
        "src/vst_controller.h"
        "src/vst_processor.h"
        "src/vie_view.h"
        "src/vst_plugin.h"
        "src/vst_plugin_factory.h"
        )
source_group("Header Files" FILES ${HEADER_FILES})

set(RESOURCES
        "src/assets/config/desktop.ini"
        "src/assets/config/default_instrument.json"
        "src/assets/config/vie.ico"
        "src/assets/view/"
        )
source_group("Resources" FILES ${RESOURCES})

set(SOURCE_FILES
        "src/application.cc"
        "src/core_logger_impl.cc"
	"src/editor_view.cc"
        "src/vst_controller.cc"
        "src/vst_processor.cc"
        "src/vie_view.cc"
        "src/vst_plugin.cc"
        "src/vst_plugin_factory.cc"
        )
source_group("Source Files" FILES ${SOURCE_FILES})

set(ALL_FILES
        ${HEADER_FILES}
        ${RESOURCES}
        ${SOURCE_FILES}
        )

add_compile_definitions(NOMINMAX)

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

if (DEBUG)
    add_compile_definitions(VIE_DEBUG)
endif ()

# Dependencies
message(STATUS "VIE> Setting-up dependencies")
add_dependencies(${PROJECT_NAME}
        vst_interfaces
	vie_core
        webview2
        wil
        )

message(STATUS "VIE> Set target C++ standard to 20")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

message(STATUS "VIE> Adding nlohman's json header directory: ${JSON_DIRECTORY}")
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_DIRECTORY})

get_target_property(VIE_CORE_INCLUDE_DIRECTORY vie_core VIE_CORE_INCLUDE_DIRECTORIES)
message(STATUS "VIE> Adding vie core header directory: ${VIE_CORE_INCLUDE_DIRECTORY}")
target_include_directories(${PROJECT_NAME} PUBLIC ${VIE_CORE_INCLUDE_DIRECTORY})

message(STATUS "VIE> Linking-up VST interfaces")
target_link_libraries(${PROJECT_NAME} vst_interfaces)

message(STATUS "VIE> Linking-up vie core")
target_link_libraries(${PROJECT_NAME} vie_core)

message(STATUS "VIE> Linking-up webview2")
target_link_libraries(${PROJECT_NAME} webview2)

message(STATUS "VIE> Linking-up wil")
target_link_libraries(${PROJECT_NAME} wil)

# Copy built VST to VST3 system folder
if (DEPLOY_TO_VST_FOLDER)
    message(STATUS "VIE> Deploying to VST standard folder")
    if (WIN32)
        add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/assets/config/Vie.ico ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/assets/config/desktop.ini ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/assets/config/default_instrument.json ${CMAKE_CURRENT_BINARY_DIR}/default_instrument.json
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/view ${CMAKE_CURRENT_BINARY_DIR}/view
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dll ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
                COMMAND attrib +s ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
                COMMAND attrib +s ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
                COMMAND attrib +s ${CMAKE_CURRENT_BINARY_DIR}/default_instrument.json
                COMMAND attrib +s ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3 "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/${PROJECT_NAME}.vst3"
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/Vie.ico"
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/desktop.ini"
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/default_instrument.json "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/default_instrument.json"
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/view "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/view"
                COMMAND attrib +s "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}"
                COMMAND endlocal
        )
    endif ()
endif ()