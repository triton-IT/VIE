message(STATUS "VIE> tests> Adding tests")

set(TEST_PROJECT_NAME VIE_tests)

project(${TEST_PROJECT_NAME})

# Source groups
set(HEADER_FILES
        "../src/boolean_parameter.hpp"
        "../src/float_parameter.hpp"
        "../src/parameter.hpp"
        "../src/components/audio_input.hpp"
        "../src/components/envelope.hpp"
        "../src/components/gain.hpp"
        "../src/components/low_pass.hpp"
        "../src/components/high_pass.hpp"
        "../src/components/midi_input.hpp"
        "../src/components/mixer.hpp"
        "../src/components/multiplier.hpp"
        "../src/components/noise.hpp"
        "../src/components/oscillator.hpp"
        "../src/components/audio_output.hpp"
		"../src/components/sample.hpp"
		"../src/components/recorder.hpp"
        )
source_group("Header Files" FILES ${HEADER_FILES})

set(SOURCE_FILES
        "../src/boolean_parameter.cpp"
        "../src/float_parameter.cpp"
        "../src/parameter.cpp"
        "../src/components/audio_input.cpp"
        "../src/components/envelope.cpp"
        "../src/components/gain.cpp"
        "../src/components/low_pass.cpp"
        "../src/components/high_pass.cpp"
        "../src/components/midi_input.cpp"
        "../src/components/mixer.cpp"
        "../src/components/multiplier.cpp"
        "../src/components/noise.cpp"
        "../src/components/oscillator.cpp"
        "../src/components/audio_output.cpp"
		"../src/components/sample.cpp"
		"../src/components/recorder.cpp"
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

#catch_discover_tests(${TEST_PROJECT_NAME})