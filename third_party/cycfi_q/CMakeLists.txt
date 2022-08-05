message(STATUS "VIE> Cycfi Q> Fetch")

include(FetchContent)

FetchContent_Declare(
    cycfi_q
    GIT_REPOSITORY https://github.com/cycfi/Q
    GIT_TAG        86142c85534169c293917c782484ee1f3406998e
)

FetchContent_GetProperties(cycfi_q)
if(NOT cycfi_q_POPULATED)
	FetchContent_Populate(cycfi_q)
endif()

message(STATUS "VIE> Cycfi Q> Add library")
add_library(cycfi_q INTERFACE)

message(STATUS "VIE> Cycfi Q> Include directories: ${cycfi_q_SOURCE_DIR}/q_lib/include/ ${cycfi_q_SOURCE_DIR}/infra/include/")
target_include_directories(cycfi_q INTERFACE ${cycfi_q_SOURCE_DIR}/q_lib/include/ ${cycfi_q_SOURCE_DIR}/infra/include/)