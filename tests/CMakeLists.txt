message(STATUS "VIE> tests> Adding tests")

set(TEST_PROJECT_NAME VIE_tests)

project(${TEST_PROJECT_NAME})

# Source groups
set(HEADER_FILES
        "../src/boolean_parameter.hpp"
        "../src/float_parameter.hpp"
        "../src/parameter.hpp"
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
        "../src/boolean_parameter.cpp"
        "../src/float_parameter.cpp"
        "../src/parameter.cpp"
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

set(TESTS_SOURCE_FILES
        "audio_in_tests.cpp"
        )
source_group("Test source Files" FILES ${TESTS_SOURCE_FILES})

set(ALL_FILES
        ${HEADER_FILES}
#        ${RESOURCES}
        ${SOURCE_FILES}
        ${TESTS_SOURCE_FILES}
        )

add_executable(${TEST_PROJECT_NAME} ${ALL_FILES})

add_dependencies(${TEST_PROJECT_NAME}
        cycfi_q
        libsndfile
        )

message(STATUS "VIE> tests>  Linking-up cycfi Q")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE cycfi_q)

message(STATUS "VIE> tests>  Linking-up libsndfile")
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE sndfile)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

message(STATUS "VIE> tests> Adding nlohman's json header directory")
set(JSON_DIRECTORY "../third_party/nlohman_json")
target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${JSON_DIRECTORY})