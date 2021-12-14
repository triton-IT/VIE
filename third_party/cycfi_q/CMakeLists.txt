message(STATUS "VIE> Cycfi Q> Fetch")

include(FetchContent)

FetchContent_Declare(
    cycfi_q
    GIT_REPOSITORY https://github.com/cycfi/Q
    GIT_TAG        b55e199a34e58eceb9fcbddb7edd6f1e58768301
)

FetchContent_GetProperties(cycfi_q)
if(NOT cycfi_q_POPULATED)
	FetchContent_Populate(cycfi_q)
	
	message(STATUS "VIE> Cycfi Q> Add library")
	add_library(cycfi_q INTERFACE)
endif()

message(STATUS "VIE> Cycfi Q> Include directories: ${cycfi_q_SOURCE_DIR}/q_lib/include/ ${cycfi_q_SOURCE_DIR}/infra/include/")
target_include_directories(cycfi_q INTERFACE ${cycfi_q_SOURCE_DIR}/q_lib/include/ ${cycfi_q_SOURCE_DIR}/infra/include/)