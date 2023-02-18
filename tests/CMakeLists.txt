message(STATUS "VIE> tests> Adding tests")

set(TEST_PROJECT_NAME VIE_tests)

project(${TEST_PROJECT_NAME})

# Source groups
set(HEADER_FILES
        "../src/application.hpp"
        "../src/bus.hpp"
        "../src/constants.hpp"
        "../src/float_parameter.hpp"
        "../src/logger.hpp"
        "../src/data_event.hpp"
        "../src/note_event.hpp"
        "../src/parameter.hpp"
        "../src/processor_definition.hpp"
        "../src/processor_module.hpp"
        "../src/processor_modules.hpp"
        "../src/processor_orchestrator.hpp"
        "../src/project.hpp"
        "../src/vst_controller.hpp"
		"../src/host_callback.hpp"
		"../src/vie_processor.hpp"
        "../src/vst_processor.hpp"
        "../src/vie_view.hpp"
        "../src/vst_plugin.hpp"
        "../src/vst_plugin_factory.hpp"
        "../src/boolean_parameter.hpp"
        "../src/modules/audio_input.hpp"
        "../src/modules/envelope.hpp"
        "../src/modules/gain.hpp"
        "../src/modules/low_pass.hpp"
        "../src/modules/high_pass.hpp"
        "../src/modules/midi_input.hpp"
        "../src/modules/mixer.hpp"
        "../src/modules/multiplier.hpp"
        "../src/modules/noise.hpp"
        "../src/modules/oscillator.hpp"
        "../src/modules/audio_output.hpp"
		"../src/modules/sample.hpp"
		"../src/modules/recorder.hpp"
        )
source_group("Header Files" FILES ${HEADER_FILES})

set(SOURCE_FILES
        "../src/bus.cpp"
        "../src/float_parameter.cpp"
        "../src/logger.cpp"
        "../src/data_event.cpp"
        "../src/note_event.cpp"
        "../src/parameter.cpp"
        "../src/processor_module.cpp"
        "../src/processor_modules.cpp"
        "../src/processor_orchestrator.cpp"
        "../src/project.cpp"
        "../src/vst_controller.cpp"
        "../src/vie_processor.cpp"
        "../src/vst_processor.cpp"
        "../src/vie_view.cpp"
        "../src/vst_plugin.cpp"
        "../src/vst_plugin_factory.cpp"
        "../src/boolean_parameter.cpp"
        "../src/modules/audio_input.cpp"
        "../src/modules/envelope.cpp"
        "../src/modules/gain.cpp"
        "../src/modules/low_pass.cpp"
        "../src/modules/high_pass.cpp"
        "../src/modules/midi_input.cpp"
        "../src/modules/mixer.cpp"
        "../src/modules/multiplier.cpp"
        "../src/modules/noise.cpp"
        "../src/modules/oscillator.cpp"
        "../src/modules/audio_output.cpp"
		"../src/modules/sample.cpp"
		"../src/modules/recorder.cpp"
        )
source_group("Source Files" FILES ${SOURCE_FILES})

if (WIN32)
    list(APPEND SOURCE_FILES "../src/windows/application.cpp")
    list(APPEND SOURCE_FILES "../src/windows/editor_view.cpp")
    list(APPEND SOURCE_FILES "../src/windows/vst_host_callback.cpp")
endif ()

set(TESTS_SOURCE_FILES
        "audio_in_tests.cpp"
        "editor_view_tests.cpp"
        )
source_group("Test source Files" FILES ${TESTS_SOURCE_FILES})

set(ALL_FILES
        ${HEADER_FILES}
#        ${RESOURCES}
        ${SOURCE_FILES}
        ${TESTS_SOURCE_FILES}
        )

if (WIN32)
    add_compile_definitions(NOMINMAX)
endif ()

add_executable(${TEST_PROJECT_NAME} ${ALL_FILES})

add_dependencies(${TEST_PROJECT_NAME}
        vst_interfaces
        cycfi_q
        webview2
        wil
        libsndfile
        )

#Add a pre-processor to be able to test private methods.
add_compile_definitions(UNIT_TESTING)

message(STATUS "VIE> Set target C++ standard to 20")
target_compile_features(${TEST_PROJECT_NAME} PRIVATE cxx_std_20)

message(STATUS "VIE> tests> Adding nlohman's json header directory")
set(JSON_DIRECTORY "../third_party/nlohman_json")
target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${JSON_DIRECTORY})

message(STATUS "VIE> tests>  Linking-up cycfi Q")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE cycfi_q)

message(STATUS "VIE> Linking-up VST interfaces")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE vst_interfaces)

message(STATUS "VIE> Linking-up webview2")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE webview2)

message(STATUS "VIE> Linking-up wil")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE wil)

message(STATUS "VIE> Linking-up libsndfile")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE sndfile)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)