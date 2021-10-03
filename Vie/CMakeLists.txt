set(PROJECT_NAME Vie)

################################################################################
# Source groups
################################################################################
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

set(Header_Files__UI_model
    "AlignModel.hpp"
    "BasicThumbModel.hpp"
    "BasicTrackModel.hpp"
    "ColorModel.hpp"
    "DialModel.hpp"
    "MarginModel.h"
    "MinSizeModel.hpp"
    "Model.hpp"
    "PaneModel.hpp"
    "SliderLabelsModel.hpp"
    "SliderMarksModel.hpp"
    "SliderModel.hpp"
    "StretchModel.hpp"
    "TileModel.hpp"
)
source_group("Header Files\\UI model" FILES ${Header_Files__UI_model})

set(Resources
    "desktop.ini"
    "ui.json"
)
source_group("Resources" FILES ${Resources})

set(Source_Files
    "Application.cpp"
    "FrequencyParameter.cpp"
    "Logger.cpp"
    "MarginModel.cpp"
    "PluginFactory.cpp"
    "VieController.cpp"
    "VieProcessor.cpp"
    "VieView.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__UI_model}
    ${Resources}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE $(ProjectName))

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "$ENV{MSBuildProjectName}"
    TARGET_NAME_RELEASE "$ENV{MSBuildProjectName}"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/Vie/lib/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Vie/lib/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CYCFI_ELEMENTS_DIR}/lib/include;"
    "${CYCFI_ELEMENTS_DIR}/lib/infra/include"
	"${CYCFI_ELEMENTS_DIR}/lib/infra/external/filesystem/include;"
	"${CYCFI_ELEMENTS_DIR}/lib/infra/external/optional-lite/include;"
	"${CYCFI_ELEMENTS_DIR}/lib/infra/external/string-view-lite/include"
    "${CYCFI_ELEMENTS_DIR}/lib/external/asio/asio/include;"
    "${CYCFI_ELEMENTS_DIR}/lib/external/cairo/include;"
    "${CYCFI_ELEMENTS_DIR}/lib/external/fontconfig/include;"
	"${CYCFI_ELEMENTS_DIR}/lib/external/freetype/include;"
    "${CYCFI_Q_DIR}/q_lib/include;"
    "${CYCFI_Q_DIR}/q_io/include;"
	"${NLOHMANN_JSON_DIR}"
    "${STEINBERG_VST3SDK_DIR}/vst3sdk;"
    "${STEINBERG_VST3SDK_DIR}/vst3sdk/vstgui4;"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG;"
        "DEBUG_LOG;"
        "DEVELOPMENT=1;"
        "SMTG_RENAME_ASSERT=1;"
        "CMAKE_INTDIR=\"Debug\";"
        "Vie_EXPORTS"
    ">"
    "$<$<CONFIG:Release>:"
        "INFRA_USE_STD_STRING_VIEW;"
        "NDEBUG;"
        "BOOST_DATE_TIME_NO_LIB;"
        "BOOST_REGEX_NO_LIB"
    ">"
    "WIN32;"
    "_WINDOWS;"
    "WIN32_LEAN_AND_MEAN;"
    "NOMINMAX;"
    "_UNICODE;"
    "ELEMENTS_HOST_UI_LIBRARY_WIN32;"
    "UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Ob0;
            /RTC1;
            /ZI;
            /GR
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /Ot
        >
        /MP;
        /std:c++17;
        /TP;
        /fp:fast;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG;
            /INCREMENTAL;
            /EXPORT:GetPluginFactory;
            /EXPORT:InitDll;
            /EXPORT:ExitDll
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /SAFESEH:NO;
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:Debug> setlocal
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/Vie.ico ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/desktop.ini ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/ui.json ${CMAKE_CURRENT_BINARY_DIR}/ui.json
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/Vie/lib/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/${PROJECT_NAME}/${PROJECT_NAME}.dll ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
    COMMAND   $<CONFIG:Debug> attrib +s ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico
    COMMAND   $<CONFIG:Debug> attrib +s ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini
    COMMAND   $<CONFIG:Debug> attrib +s ${CMAKE_CURRENT_BINARY_DIR}/ui.json
    COMMAND   $<CONFIG:Debug> attrib +s ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vst3 "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/${PROJECT_NAME}.vst3"
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Vie.ico "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/Vie.ico"
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/desktop.ini "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/desktop.ini"
    COMMAND   $<CONFIG:Debug> ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ui.json "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}/ui.json"
    COMMAND   $<CONFIG:Debug> attrib +s "$ENV{ProgramW6432}/Common Files/VST3/${PROJECT_NAME}"
    COMMAND   $<CONFIG:Debug> endlocal
    COMMENT "
      "
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    base
    elements
    pluginterfaces
    sdk
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
		"dbghelp;"
    ">"
    "base;"
    "sdk;"
    "pluginterfaces;"
	"cairo;"
	"fontconfig;"
	"freetype;"
	"elements;"
	"user32;"
	"gdi32;"
	"winmm;"
	"imm32;"
	"ole32;"
	"oleaut32;"
	"version;"
	"uuid;"
	"ws2_32;"
	"d3dcompiler;"
	"d3d9;"
	"kernel32;"
	"winspool;"
	"shell32;"
	"comdlg32;"
	"advapi32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${STEINBERG_VST3SDK_DIR}/build/lib/Debug;"
        "${CYCFI_ELEMENTS_DIR}/build/lib/Debug;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/cairo/lib/x64;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/fontconfig/x64;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/freetype/win64"
    ">"
    "$<$<CONFIG:Release>:"
        "${STEINBERG_VST3SDK_DIR}/build/lib/Debug;"
        "${CYCFI_ELEMENTS_DIR}/build/lib/Debug;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/cairo/lib/x64;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/fontconfig/x64;"
        "${CYCFI_ELEMENTS_DIR}/lib/external/freetype/win64"
    ">"
)

