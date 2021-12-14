message(STATUS "VIE> GLM> Fetch")

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        0.9.9.8
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)

	message(STATUS "VIE> GLM> Add library")
	add_library(glm INTERFACE)
endif()

message(STATUS "VIE> GLM> Include directory: " ${glm_SOURCE_DIR})
target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})