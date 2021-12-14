set(PROJECT_NAME VIE)

# Source groups
set(Header_Files
    "Application.h"
    "Constants.h"
    "FrequencyParameter.h"
    "IParameterListener.h"
    "Logger.h"
    "VieController.h"
    "VieProcessor.h"
    "VieView.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resources
    "desktop.ini"
    "ui.json"
)
source_group("Resources" FILES ${Resources})

set(Source_Files
    "FrequencyParameter.cpp"
    "Logger.cpp"
    "PluginFactory.cpp"
    "VieController.cpp"
    "VieProcessor.cpp"
    "VieView.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
	${Header_Files}
	${Resources}
	${Source_Files}
)

if( WIN32 )
	list(APPEND ALL_FILES "windows/Application.cpp")
endif()

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

# Dependencies
message(STATUS "VIE> C++> Setting-up dependencies")
add_dependencies(${PROJECT_NAME}
    tinyobjloader
	base
	pluginterfaces
	sdk
	diligent_core
)

message(STATUS "VIE> C++> Linking-up Steinberg VST SDK")
target_link_libraries(${PROJECT_NAME} base pluginterfaces sdk)

message(STATUS "VIE> C++> Linking-up tinyObjLoader")
target_link_libraries(${PROJECT_NAME} tinyobjloader)

message(STATUS "VIE> C++> Linking-up STB")
target_link_libraries(${PROJECT_NAME} stb)

message(STATUS "VIE> C++> Linking-up cycfi Q")
target_link_libraries(${PROJECT_NAME} cycfi_q)

message(STATUS "VIE> C++> Linking-up Diligent engine")

target_link_libraries(${PROJECT_NAME} Diligent-GraphicsEngineVk-shared)

# Copy built VST to VST3 system folder
if( DEPLOY_TO_VST_FOLDER )
	message(STATUS "VIE> Deploying to VST standard folder")
	if( WIN32 )
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND   setlocal
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/Vie.ico ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/desktop.ini ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/ui.json ${CMAKE_CURRENT_BINARY_DIR}/ui.json
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/src/$<CONFIG>/${PROJECT_NAME}.dll ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
			COMMAND   attrib +s ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
			COMMAND   attrib +s ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
			COMMAND   attrib +s ${CMAKE_CURRENT_BINARY_DIR}/ui.json
			COMMAND   attrib +s ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3 "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/${PROJECT_NAME}.vst3"
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/Vie.ico"
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/desktop.ini"
			COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ui.json "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/ui.json"
			COMMAND   attrib +s "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}"
			COMMAND   endlocal
		)
	endif()
endif()

message(STATUS "VIE> Compiling shaders.")
message(STATUS "VIE> Shaders> Find GLSL Validator")
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

set(SHADER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/assets/shaders")

message(STATUS "VIE> Shaders> Getting shaders in ${SHADER_SOURCE_DIR}")
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
    )

message(STATUS "VIE> Shaders> Creating target directory for shaders.")
set(SPIRV_OUTPUT_FOLDER "${PROJECT_BINARY_DIR}/assets/shaders")
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_FOLDER}"
)
  
foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "VIE> Shaders> Iterating on shader ${GLSL}")  
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${SPIRV_OUTPUT_FOLDER}/${FILE_NAME}.spv")
  
  message(STATUS "VIE> Shaders> Building shader ${GLSL} to ${SPIRV}")
  add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)