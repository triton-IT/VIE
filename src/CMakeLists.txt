set(PROJECT_NAME VIE)

project(${PROJECT_NAME})

set(JSON_DIRECTORY
        "../third_party/nlohman_json"
        )

# Source groups
set(HEADER_FILES
        "application.hpp"
        "bus.hpp"
        "constants.hpp"
        "float_parameter.hpp"
        "logger.hpp"
        "data_event.hpp"
        "note_event.hpp"
        "parameter.hpp"
        "processor_definition.hpp"
        "processor_module.hpp"
        "processor_modules.hpp"
        "processor_orchestrator.hpp"
        "vst_controller.hpp"
        "vie_processor.hpp"
        "vst_processor.hpp"
        "vie_view.hpp"
        "vst_plugin.hpp"
        "vst_plugin_factory.hpp"
        "boolean_parameter.hpp"
        "modules/audio_input.hpp"
        "modules/envelope.hpp"
        "modules/gain.hpp"
        "modules/low_pass.hpp"
        "modules/high_pass.hpp"
        "modules/midi_input.hpp"
        "modules/mixer.hpp"
        "modules/multiplier.hpp"
        "modules/noise.hpp"
        "modules/oscillator.hpp"
        "modules/audio_output.hpp"
		"modules/sample.hpp"
		"modules/recorder.hpp"
        )
source_group("Header Files" FILES ${HEADER_FILES})

if (WIN32)
    list(APPEND HEADER_FILES "windows/editor_view.hpp")
endif ()

set(RESOURCES
        "assets/config/desktop.ini"
        "assets/config/default_instrument.json"
        "assets/config/vie.ico"
        "assets/view/"
        )
source_group("Resources" FILES ${RESOURCES})

set(SOURCE_FILES
        "bus.cpp"
        "float_parameter.cpp"
        "logger.cpp"
        "data_event.cpp"
        "note_event.cpp"
        "parameter.cpp"
        "processor_module.cpp"
        "processor_modules.cpp"
        "processor_orchestrator.cpp"
        "vst_controller.cpp"
        "vie_processor.cpp"
        "vst_processor.cpp"
        "vie_view.cpp"
        "vst_plugin.cpp"
        "vst_plugin_factory.cpp"
        "boolean_parameter.cpp"
        "modules/audio_input.cpp"
        "modules/envelope.cpp"
        "modules/gain.cpp"
        "modules/low_pass.cpp"
        "modules/high_pass.cpp"
        "modules/midi_input.cpp"
        "modules/mixer.cpp"
        "modules/multiplier.cpp"
        "modules/noise.cpp"
        "modules/oscillator.cpp"
        "modules/audio_output.cpp"
		"modules/sample.cpp"
		"modules/recorder.cpp"
        )
source_group("Source Files" FILES ${SOURCE_FILES})

if (WIN32)
    list(APPEND SOURCE_FILES "windows/application.cpp")
    list(APPEND SOURCE_FILES "windows/editor_view.cpp")
endif ()

set(ALL_FILES
        ${HEADER_FILES}
        ${RESOURCES}
        ${SOURCE_FILES}
        )

if (WIN32)
    add_compile_definitions(NOMINMAX)
endif ()

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

if (DEBUG)
    add_compile_definitions(VIE_DEBUG)
endif ()

# Dependencies
message(STATUS "VIE> Setting-up dependencies")
add_dependencies(${PROJECT_NAME}
        vst_interfaces
        cycfi_q
        webview2
        wil
        libsndfile
        )

message(STATUS "VIE> Set target C++ standard to 17")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

message(STATUS "VIE> Adding nlohman's json header directory")
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_DIRECTORY})

message(STATUS "VIE> Linking-up VST interfaces")
target_link_libraries(${PROJECT_NAME} vst_interfaces)

message(STATUS "VIE> Linking-up cycfi Q")
target_link_libraries(${PROJECT_NAME} cycfi_q)

message(STATUS "VIE> Linking-up webview2")
target_link_libraries(${PROJECT_NAME} webview2)

message(STATUS "VIE> Linking-up wil")
target_link_libraries(${PROJECT_NAME} wil)

message(STATUS "VIE> Linking-up libsndfile")
target_link_libraries(${PROJECT_NAME} sndfile)

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