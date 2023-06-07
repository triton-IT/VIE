cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

# set(vst3sdk_SOURCE_DIR "/Users/regis/vst3sdk")

set(SMTG_MAC TRUE)

project(VIE_AUV3
    VERSION 1.0.0.0 
    DESCRIPTION "VIE_AUV3 VST 3 Plug-in"
)
# add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
# smtg_enable_vst3_sdk()
function(smtg_target_set_debug_executable target executable)
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set_target_properties(${target}
            PROPERTIES
                XCODE_GENERATE_SCHEME       YES
                XCODE_SCHEME_EXECUTABLE     "${executable}"
        )
        if(ARGC GREATER 2 AND ARGV2)
            set_target_properties(${target}
                PROPERTIES
                    XCODE_SCHEME_ARGUMENTS  "${ARGV2}"
            )
        endif()
    endif()

    if(MSVC)
        set_target_properties(${target}
            PROPERTIES
                VS_DEBUGGER_COMMAND         "${executable}"
        )
        if(ARGC GREATER 2 AND ARGV2)
            set_target_properties(${target}
                PROPERTIES
                    VS_DEBUGGER_COMMAND_ARGUMENTS "${ARGV2}"
            ) 
        endif()
    endif(MSVC)
endfunction(smtg_target_set_debug_executable)

function(smtg_target_set_bundle target)
    if(NOT SMTG_MAC)
        message(FATAL_ERROR "[SMTG] smtg_target_set_bundle only works on macOS, use it in an if(SMTG_MAC) block")
    endif(NOT SMTG_MAC)

    set(options 
        PREPROCESS
    )
    set(oneValueArgs 
        EXTENSION 
        INFOPLIST
        INFOPLIST_IN
        BUNDLE_IDENTIFIER
        COMPANY_NAME
    )
    set(multiValueArgs 
        RESOURCES 
        PREPROCESSOR_FLAGS 
    )
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "[SMTG] The following parameters are unrecognized: ${ARG_UNPARSED_ARGUMENTS}")
    endif(ARG_UNPARSED_ARGUMENTS)

    # Adding the bundle resources to the target sources creates a warning, see https://cmake.org/Bug/view.php?id=15272
    target_sources(${target} 
        PRIVATE
            ${ARG_RESOURCES}
    )

    get_target_property(type ${target} TYPE)
    if(type STREQUAL MODULE_LIBRARY)
        set_target_properties(${target}
            PROPERTIES
                BUNDLE TRUE
        )
    elseif(type STREQUAL EXECUTABLE)
        set_target_properties(${target}
            PROPERTIES
                MACOSX_BUNDLE TRUE
        )
    endif()

    if(ARG_EXTENSION)
        if(XCODE)
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_WRAPPER_EXTENSION ${ARG_EXTENSION}
            )
        else()
            set_target_properties(${target}
                PROPERTIES
                    BUNDLE_EXTENSION ${ARG_EXTENSION}
            )
        endif(XCODE)
    endif(ARG_EXTENSION)

    if(ARG_RESOURCES)
        set_source_files_properties(${ARG_RESOURCES}
            PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources
        )
    endif(ARG_RESOURCES)

    if(ARG_INFOPLIST AND XCODE)
        set_target_properties(${target}
            PROPERTIES
                XCODE_ATTRIBUTE_INFOPLIST_FILE "${ARG_INFOPLIST}"
        )
        if(ARG_PREPROCESS)
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS "YES"
            )
        endif(ARG_PREPROCESS)
        if(ARG_PREPROCESSOR_FLAGS)
            set_target_properties(${target} 
                PROPERTIES
                    XCODE_ATTRIBUTE_INFOPLIST_OTHER_PREPROCESSOR_FLAGS "${ARG_PREPROCESSOR_FLAGS}"
            )
        endif(ARG_PREPROCESSOR_FLAGS)
    elseif(XCODE)
        # Build Copyright string:
        string(TIMESTAMP BUNDLE_INFO_STRING_YEAR "%Y")
        set(BUNDLE_COPYRIGHT "© ${BUNDLE_INFO_STRING_YEAR} ${ARG_COMPANY_NAME}.")

        # Check for custom info.plist template:
        if(ARG_INFOPLIST_IN)
            # dest Info.plist.in template file in cmake format
            # see:  https://duckduckgo.com/?q=MACOSX_BUNDLE_INFO_PLIST
            set(SMTG_BUNDLE_INFO_PLIST "${PROJECT_BINARY_DIR}/smtg_mac/Info.plist.in")
            # Configure SMTG_BUNDLE_INFO_PLIST to cmake MACOSX_BUNDLE_INFO_PLIST template
            # Use @VAR@ for variables not defined in MACOSX_BUNDLE_INFO_PLIST
            configure_file("${ARG_INFOPLIST_IN}" "${SMTG_BUNDLE_INFO_PLIST}" @ONLY)

            set_target_properties(${smtg_target}
                PROPERTIES
                    MACOSX_BUNDLE_INFO_PLIST ${SMTG_BUNDLE_INFO_PLIST}
            )        
        endif()

        set_target_properties(${target}
            PROPERTIES
                MACOSX_BUNDLE_GUI_IDENTIFIER ${ARG_BUNDLE_IDENTIFIER}
                MACOSX_BUNDLE_COPYRIGHT ${BUNDLE_COPYRIGHT}
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
                MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
                MACOSX_BUNDLE_INFO_STRING ${PROJECT_VERSION}
        )
    endif()

    if(ARG_BUNDLE_IDENTIFIER)
        if(XCODE)
        	set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${ARG_BUNDLE_IDENTIFIER}"
            )
        endif(XCODE)
	endif(ARG_BUNDLE_IDENTIFIER)
endfunction()

function(smtg_target_configure_version_file target)

    set(SMTG_PROJECT_VERSION "")

    if(NOT "${PROJECT_VERSION_MAJOR}" STREQUAL "")
        string(APPEND SMTG_PROJECT_VERSION "${PROJECT_VERSION_MAJOR}")
    endif()
    if(NOT "${PROJECT_VERSION_MINOR}" STREQUAL "")
        string(APPEND SMTG_PROJECT_VERSION ".${PROJECT_VERSION_MINOR}")
    endif()
    if(NOT "${PROJECT_VERSION_PATCH}" STREQUAL "")
        string(APPEND SMTG_PROJECT_VERSION ".${PROJECT_VERSION_PATCH}")
    endif()

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/projectversion.h.in projectversion.h)

    target_include_directories(${target} PUBLIC
        "${PROJECT_BINARY_DIR}"
    )
endfunction()

function(smtg_target_add_plugin_resource target input_file)
    if(SMTG_LINUX OR (SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS))
        get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
        get_target_property(PLUGIN_PACKAGE_RESOURCES ${target} SMTG_PLUGIN_PACKAGE_RESOURCES)
        set(destination_folder "${PLUGIN_PACKAGE_PATH}/${PLUGIN_PACKAGE_RESOURCES}")
        if(ARGC GREATER 2 AND ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()

        # Make the incoming path absolute.
        get_filename_component(absolute_input_file_path "${input_file}" ABSOLUTE)

        # Extract the filename and its extension
        get_filename_component(file_name_with_extension "${input_file}" NAME)

        # Create absolute output file path
        set(absolute_output_file_path "${destination_folder}/${file_name_with_extension}")

        # Add the file as a source to the target
        target_sources(${target}
            PRIVATE
                ${input_file}
        )

        if(MSVC)
            # Hacky workaround: replace all SMTG_PLUGIN_PACKAGE_NAME (e.g. again.vst3) 
            # occurences by a MSVS macro $(TargetFileName). Using the cmake only approach does not work.
            get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)
            string(REPLACE "${PLUGIN_PACKAGE_NAME}" "$(TargetFileName)" absolute_output_file_path ${absolute_output_file_path})
        endif()

        # Create a custom build tool for the specific file
        add_custom_command(
            OUTPUT  ${absolute_output_file_path}
            MAIN_DEPENDENCY ${absolute_input_file_path}
            COMMAND ${CMAKE_COMMAND} 
                -E copy_if_different
                    ${absolute_input_file_path}
                    ${absolute_output_file_path}
            COMMAND ${CMAKE_COMMAND} 
                -E echo 
                    "[SMTG] Copied ${absolute_input_file_path} to ${absolute_output_file_path}"
        )
    elseif(SMTG_MAC)
        target_sources(${target}
            PRIVATE
                ${input_file}
        )
        set(destination_folder "Resources")
        if(ARGC GREATER 2 AND ARGV2)
            set(destination_folder "${destination_folder}/${ARGV2}")
        endif()

        set_source_files_properties(${input_file} 
            PROPERTIES 
                MACOSX_PACKAGE_LOCATION "${destination_folder}"
        )
    endif(SMTG_LINUX OR (SMTG_WIN AND SMTG_CREATE_BUNDLE_FOR_WINDOWS))
endfunction(smtg_target_add_plugin_resource)

function(smtg_target_add_plugin_snapshots target)
    cmake_parse_arguments(
        PARSED_ARGS # Prefix of output variables e.g. PARSED_ARGS_RESOURCES
        ""          # List of names for boolean arguments
        ""          # List of names for mono-valued arguments
        "RESOURCES" # List of names for multi-valued arguments resp. lists
        ${ARGN}     # Arguments of the function to parse
    )
    get_target_property(PLUGIN_PACKAGE_SNAPSHOTS ${target} SMTG_PLUGIN_PACKAGE_SNAPSHOTS)
  
    foreach(rsrc ${PARSED_ARGS_RESOURCES})
        smtg_target_add_plugin_resource (${target} ${rsrc} ${PLUGIN_PACKAGE_SNAPSHOTS}) 
    endforeach()
endfunction(smtg_target_add_plugin_snapshots)

function(smtg_target_create_link_to_plugin target destination)
if(${destination} STREQUAL "")
    message(FATAL_ERROR "[SMTG] no destination defined!")
endif()

get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)
get_target_property(TARGET_SOURCE       ${target} SMTG_PLUGIN_PACKAGE_PATH)
set(TARGET_DESTINATION ${destination})

if(SMTG_WIN)
    get_target_property(PLUGIN_BINARY_DIR ${target} SMTG_PLUGIN_BINARY_DIR)

    file(TO_CMAKE_PATH "${TARGET_DESTINATION}/${PLUGIN_PACKAGE_NAME}" SRC_NATIVE_PATH)
    file(TO_CMAKE_PATH "${PLUGIN_BINARY_DIR}/Debug/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_DEBUG)
    file(TO_CMAKE_PATH "${PLUGIN_BINARY_DIR}/Release/${PLUGIN_PACKAGE_NAME}" TARGET_DESTINATION_RELEASE)

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo [SMTG] Delete previous link...
        COMMAND ${CMAKE_COMMAND} -E rm -rf "${SRC_NATIVE_PATH}"
        COMMAND ${CMAKE_COMMAND} -E echo [SMTG] Creation of the new link...
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            "$<$<CONFIG:Debug>:${TARGET_DESTINATION_DEBUG}>"
            "$<$<CONFIG:Release>:${TARGET_DESTINATION_RELEASE}>"
            "${SRC_NATIVE_PATH}"
        COMMAND ${CMAKE_COMMAND} -E echo [SMTG] Finished.
    )
else()
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_DESTINATION}"
        COMMAND ln -svfF "${TARGET_SOURCE}" "${TARGET_DESTINATION}"
    )
endif(SMTG_WIN)
endfunction(smtg_target_create_link_to_plugin)

function(smtg_get_default_vst3_path)
if(SMTG_WIN)
    set(win64Found "1")
    if(NOT ${CMAKE_GENERATOR_PLATFORM} STREQUAL "")
        string(FIND ${CMAKE_GENERATOR_PLATFORM} x64 win64Found)
        if(${win64Found} EQUAL -1)
            string(FIND ${CMAKE_GENERATOR_PLATFORM} ARM64 win64Found)
        endif()
    elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017" OR 
           ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015" OR 
           ${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013" OR 
           ${CMAKE_GENERATOR} STREQUAL "Visual Studio 11 2012" OR 
           ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10 2010" OR 
           ${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
                set(win64Found "-1")
    else()
        string(FIND ${CMAKE_GENERATOR} Win32 win32Found)
        if(NOT ${win32Found} EQUAL -1)
            set(win64Found "-1")
        endif()
    endif()
    if(${win64Found} EQUAL -1) # for 32bits OS
        if(EXISTS "$ENV{PROGRAMFILES} (x86)")
            set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{PROGRAMFILES} (x86)\\Common Files\\VST3" PARENT_SCOPE)
        else()
            set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{PROGRAMFILES}\\Common Files\\VST3" PARENT_SCOPE)
        endif()
    else()  # for 64bits OS
        #FOLDERID_UserProgramFilesCommon
        if(SMTG_PLUGIN_TARGET_USER_PROGRAM_FILES_COMMON)
            set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{LOCALAPPDATA}\\Programs\\Common\\VST3" PARENT_SCOPE)
        else()
            set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{CommonProgramW6432}\\VST3" PARENT_SCOPE)
        endif(SMTG_PLUGIN_TARGET_USER_PROGRAM_FILES_COMMON)
    endif()
elseif(SMTG_MAC)
    set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{HOME}/Library/Audio/Plug-Ins/VST3" PARENT_SCOPE)
elseif(SMTG_LINUX)
    set(SMTG_PLUGIN_TARGET_DEFAULT_PATH "$ENV{HOME}/.vst3" PARENT_SCOPE)
else()
    message(FATAL_ERROR "[SMTG] unknown platform")
endif(SMTG_WIN)
endfunction(smtg_get_default_vst3_path)

function(smtg_get_vst3_path)
    # here you can define the VST3 Plug-ins folder (it will be created), SMTG_PLUGIN_TARGET_DEFAULT_PATH will be set
    smtg_get_default_vst3_path()

    set(SMTG_PLUGIN_TARGET_USER_PATH "" CACHE PATH "Here you can redefine the VST3 Plug-ins folder")
    if(NOT ${SMTG_PLUGIN_TARGET_USER_PATH} STREQUAL "")
        set(PLUGIN_TARGET_PATH ${SMTG_PLUGIN_TARGET_USER_PATH})
    else()
        set(PLUGIN_TARGET_PATH ${SMTG_PLUGIN_TARGET_DEFAULT_PATH})
    endif()
    
    if(NOT ${PLUGIN_TARGET_PATH} STREQUAL "" AND SMTG_CREATE_PLUGIN_LINK)
        if(NOT EXISTS ${PLUGIN_TARGET_PATH})
            message(STATUS "[SMTG] Create folder: " ${PLUGIN_TARGET_PATH})
            if(SMTG_WIN)
                smtg_create_directory_as_admin_win(${PLUGIN_TARGET_PATH})
            else()
                file(MAKE_DIRECTORY ${PLUGIN_TARGET_PATH})
            endif(SMTG_WIN)
        endif(NOT EXISTS ${PLUGIN_TARGET_PATH})
    endif(NOT ${PLUGIN_TARGET_PATH} STREQUAL "" AND SMTG_CREATE_PLUGIN_LINK)
    if(EXISTS ${PLUGIN_TARGET_PATH})
        message(STATUS "[SMTG] SMTG_PLUGIN_TARGET_PATH is set to: " ${PLUGIN_TARGET_PATH})
    else()
        message(STATUS "[SMTG] SMTG_PLUGIN_TARGET_PATH is not set!")
    endif(EXISTS ${PLUGIN_TARGET_PATH})
    set(SMTG_PLUGIN_TARGET_PATH ${PLUGIN_TARGET_PATH} PARENT_SCOPE)
endfunction(smtg_get_vst3_path)

function(smtg_target_run_vst_validator target)
    if(TARGET validator)
        message(STATUS "[SMTG] Setup running validator for ${target}")
        add_dependencies(${target} validator)
        if(SMTG_WIN)
            set(TARGET_PATH $<TARGET_FILE:${target}>)
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo [SMTG] Validator started...
                COMMAND 
                    $<TARGET_FILE:validator> 
                    "${TARGET_PATH}" 
                    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                COMMAND ${CMAKE_COMMAND} -E echo [SMTG] Validator finished.
            )
        else()
            get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND 
                    $<TARGET_FILE:validator> 
                    $<$<CONFIG:Debug>:${PLUGIN_PACKAGE_PATH}>
                    $<$<CONFIG:Release>:${PLUGIN_PACKAGE_PATH}> 
                    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            )
        endif(SMTG_WIN)
    endif(TARGET validator)
endfunction(smtg_target_run_vst_validator)

function(smtg_target_create_module_info_file target)   
    set(oneValueArgs MODULEINFO_COMPATIBILITY)
    cmake_parse_arguments(PARSE_ARGV 1 PARAMS "" "${oneValueArgs}" "")
   
    if(NOT SMTG_CREATE_MODULE_INFO)
        return()
    endif()
    if(SMTG_WIN AND NOT SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        message(WARNING "[SMTG] No moduleinfo.json file for ${target} generated. Consider to enable SMTG_CREATE_BUNDLE_FOR_WINDOWS.")
        return()
    endif()

    message(STATUS "[SMTG] Setup running moduleinfotool for ${target}")

    add_dependencies(${target} moduleinfotool)

    get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
    set(SMTG_MODULEINFO_PATH_INSIDE_BUNDLE Contents/moduleinfo.json)

    if(DEFINED PARAMS_MODULEINFO_COMPATIBILITY)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND 
                $<TARGET_FILE:moduleinfotool> -create -version ${PROJECT_VERSION} 
                -path "${PLUGIN_PACKAGE_PATH}"
                -compat "${CMAKE_CURRENT_SOURCE_DIR}/${PARAMS_MODULEINFO_COMPATIBILITY}"
                -output "${PLUGIN_PACKAGE_PATH}/${SMTG_MODULEINFO_PATH_INSIDE_BUNDLE}"
                WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    else()
#         add_custom_command(
#             TARGET ${target} POST_BUILD
#             COMMAND 
#                 $<TARGET_FILE:moduleinfotool> -create -version ${PROJECT_VERSION} 
#                 -path "${PLUGIN_PACKAGE_PATH}"
#                 -output "${PLUGIN_PACKAGE_PATH}/${SMTG_MODULEINFO_PATH_INSIDE_BUNDLE}"
#                 WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
#         )
    endif()
    if(SMTG_MAC)
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND 
                codesign -f -s "-" -v "${PLUGIN_PACKAGE_PATH}/${SMTG_MODULEINFO_PATH_INSIDE_BUNDLE}"
                WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    endif()
endfunction()

function(smtg_target_create_resources_folder target)
    get_target_property(PLUGIN_PACKAGE_PATH ${target} SMTG_PLUGIN_PACKAGE_PATH)
    get_target_property(PLUGIN_PACKAGE_RESOURCES ${target} SMTG_PLUGIN_PACKAGE_RESOURCES)
    set(resources_folder "${PLUGIN_PACKAGE_PATH}/${PLUGIN_PACKAGE_RESOURCES}")
    add_custom_command(
            TARGET ${target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${resources_folder}"
    )
endfunction()

function(smtg_target_codesign target)
    if(XCODE AND (NOT SMTG_DISABLE_CODE_SIGNING))
        if(ARGC GREATER 2)
            set(team "${ARGV1}")
            set(identity "${ARGV2}")
            message(STATUS "[SMTG] Codesign ${target} with team '${team}' and identity '${identity}")
            set(SMTG_CODESIGN_ATTRIBUTES 
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM    ${team}
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY  "${identity}"
                XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "${SMTG_XCODE_OTHER_CODE_SIGNING_FLAGS}"
            )
        else()
            message(STATUS "[SMTG] Codesign ${target} for local machine only")
            set(SMTG_CODESIGN_ATTRIBUTES 
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
            )
        endif(ARGC GREATER 2)
        set_target_properties(${target}
            PROPERTIES
                ${SMTG_CODESIGN_ATTRIBUTES}
        )
		if(SMTG_XCODE_MANUAL_CODE_SIGN_STYLE)
			set_target_properties(${target}
				PROPERTIES
					XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
			)
		endif(SMTG_XCODE_MANUAL_CODE_SIGN_STYLE)
		if(SMTG_MAC AND (XCODE_VERSION VERSION_GREATER_EQUAL 12))
			# make sure that the executable is signed before cmake post build commands are run as the
			# Xcode code-sign step is run after the post build commands are run which would prevent
			# using the target output on system where everything needs to be code-signed.
			target_link_options(${target} PRIVATE LINKER:-adhoc_codesign)
		endif()
    endif(XCODE AND (NOT SMTG_DISABLE_CODE_SIGNING))
endfunction(smtg_target_codesign)

function(smtg_target_setup_universal_binary target)
    if(SMTG_MAC)
        if(SMTG_BUILD_UNIVERSAL_BINARY)
            if(XCODE_VERSION VERSION_GREATER_EQUAL 12)
                set_target_properties(${target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_OSX_ARCHITECTURES   "x86_64;arm64;arm64e"
                        XCODE_ATTRIBUTE_ARCHS               "$(ARCHS_STANDARD_64_BIT)"
                )
            else()
                set_target_properties(${target}
                    PROPERTIES
                        XCODE_ATTRIBUTE_OSX_ARCHITECTURES   "x86_64;i386"
                        XCODE_ATTRIBUTE_ARCHS               "$(ARCHS_STANDARD_32_64_BIT)"
                )
            endif()
            set_target_properties(${target}
                PROPERTIES
                    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH        "$<$<CONFIG:Debug>:YES>$<$<CONFIG:Release>:NO>"
            )
        endif(SMTG_BUILD_UNIVERSAL_BINARY)
    endif(SMTG_MAC)
endfunction(smtg_target_setup_universal_binary)

function(smtg_target_add_library_main target)
    if(public_sdk_SOURCE_DIR)
        if(SMTG_MAC)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/macmain.cpp
            )
        elseif(SMTG_WIN)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/dllmain.cpp
            )
        elseif(SMTG_LINUX)
            target_sources (${target} 
                PRIVATE 
                    ${public_sdk_SOURCE_DIR}/source/main/linuxmain.cpp
            )
        endif(SMTG_MAC)
    endif(public_sdk_SOURCE_DIR)
endfunction(smtg_target_add_library_main)

function(smtg_target_make_plugin_package target pkg_name extension)
    # smtg_target_check_project_version("${target}")
    if(${pkg_name} STREQUAL "")
        set(pkg_name ${target})
    endif()
    set(pkg_extension ${extension})
    if(SMTG_WIN)
        if(${extension} STREQUAL "bundle")
            set(extension dll)
            if(NOT SMTG_CREATE_BUNDLE_FOR_WINDOWS)
                set(pkg_extension dll)
            endif(NOT SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        endif()
    endif(SMTG_WIN)
    string(TOUPPER ${extension} PLUGIN_EXTENSION_UPPER)

    if(SMTG_CUSTOM_BINARY_LOCATION)
        set(SMTG_PLUGIN_BINARY_LOCATION ${SMTG_CUSTOM_BINARY_LOCATION})
    else()
        set(SMTG_PLUGIN_BINARY_LOCATION ${CMAKE_BINARY_DIR})
    endif(SMTG_CUSTOM_BINARY_LOCATION)

    set_target_properties(${target}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY        ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
            SMTG_PLUGIN_BINARY_DIR          ${SMTG_PLUGIN_BINARY_LOCATION}/${PLUGIN_EXTENSION_UPPER}
            SMTG_PLUGIN_EXTENSION           ${extension}
            SMTG_PLUGIN_PACKAGE_NAME        ${pkg_name}.${pkg_extension}
            SMTG_PLUGIN_PACKAGE_CONTENTS    Contents
            SMTG_PLUGIN_PACKAGE_RESOURCES   Contents/Resources
            SMTG_PLUGIN_PACKAGE_SNAPSHOTS   Snapshots
    )

    get_target_property(PLUGIN_BINARY_DIR   ${target} SMTG_PLUGIN_BINARY_DIR)
    get_target_property(PLUGIN_EXTENSION    ${target} SMTG_PLUGIN_EXTENSION)
    get_target_property(PLUGIN_PACKAGE_NAME ${target} SMTG_PLUGIN_PACKAGE_NAME)

    smtg_target_add_library_main(${target})
    
    if(SMTG_CXX_STANDARD)
        target_compile_features(${target}
            PRIVATE
                cxx_std_${SMTG_CXX_STANDARD}
        )
    endif(SMTG_CXX_STANDARD)

    if(SMTG_MAC)
        set_target_properties(${target}
            PROPERTIES
                BUNDLE TRUE
        )
        if(XCODE)
            set_target_properties(${target}
                PROPERTIES 
                    XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE           YES
                    XCODE_ATTRIBUTE_WRAPPER_EXTENSION               ${PLUGIN_EXTENSION}
                    XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS  YES
                    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT        $<$<CONFIG:Debug>:dwarf>$<$<CONFIG:Release>:dwarf-with-dsym>
                    XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING		$<$<CONFIG:Debug>:NO>$<$<CONFIG:Release>:YES>
            )
            if(SMTG_CUSTOM_BINARY_LOCATION)
              set_target_properties(${target}
                PROPERTIES 
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME}
              )
            else()
              set_target_properties(${target}
                PROPERTIES 
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME}
              )
            endif(SMTG_CUSTOM_BINARY_LOCATION)
        else()
            set_target_properties(${target}
                PROPERTIES 
                    BUNDLE_EXTENSION            ${PLUGIN_EXTENSION}
                    LIBRARY_OUTPUT_DIRECTORY    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}
                    SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
            )
        endif(XCODE)

        target_link_libraries(${target}
            PRIVATE
                "-framework CoreFoundation"
        )
        smtg_target_setup_universal_binary(${target})
        smtg_target_codesign(${target} ${SMTG_IOS_DEVELOPMENT_TEAM} "${SMTG_CODE_SIGN_IDENTITY_MAC}")

    elseif(SMTG_WIN)
        if(SMTG_CUSTOM_BINARY_LOCATION)
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME})
        else()
            set(PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${PLUGIN_PACKAGE_NAME})
        endif(SMTG_CUSTOM_BINARY_LOCATION)
        set_target_properties(${target}
            PROPERTIES 
                SUFFIX                      .${PLUGIN_EXTENSION}
                SMTG_PLUGIN_PACKAGE_PATH    ${PLUGIN_PACKAGE_PATH}
        )
        if(MSVC)
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set_target_properties(${target}
                    PROPERTIES 
                        LINK_FLAGS          /EXPORT:GetPluginFactory
                )
            endif()
        endif(MSVC)
        if(MINGW) # no "lib" prefix for MinGw
            set_target_properties(${target}
                PROPERTIES 
                    PREFIX                  ""
            )   
        endif(MINGW)
        
        # In order not to have the PDB inside the Plug-in package in release builds, 
        # we specify a different location (only for MSVC).
        if(MSVC)
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                set(WIN_PDB WIN_PDB32)
            else()
                set(WIN_PDB WIN_PDB64)
            endif()
            set_target_properties(${target}
                PROPERTIES
                    PDB_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${WIN_PDB}
            )
        endif(MSVC)

        # Create Bundle on Windows
        if(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
            get_target_property(WIN_ARCHITECTURE_NAME ${target} SMTG_WIN_ARCHITECTURE_NAME)

            # When using LIBRARY_OUTPUT_DIRECTORY, cmake adds another /Debug resp. /Release folder at the end of the path.
            # In order to prevent cmake from doing that we set LIBRARY_OUTPUT_DIRECTORY_DEBUG and LIBRARY_OUTPUT_DIRECTORY_RELEASE
            # (or LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPERCASE}) directly.
            get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
            foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
                if(SMTG_CUSTOM_BINARY_LOCATION)
                    set_target_properties(${target}
                        PROPERTIES 
                            LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PLUGIN_BINARY_DIR}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                    )
                else()
                    set_target_properties(${target}
                        PROPERTIES 
                            LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PLUGIN_BINARY_DIR}/${OUTPUTCONFIG}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${WIN_ARCHITECTURE_NAME}
                    )
                endif(SMTG_CUSTOM_BINARY_LOCATION)
            endforeach()
            if(EXISTS ${SMTG_PACKAGE_ICON_PATH})
                smtg_target_add_folder_icon(${target} ${SMTG_PACKAGE_ICON_PATH})
            endif()
        endif(SMTG_CREATE_BUNDLE_FOR_WINDOWS)
        # Disable warning LNK4221: "This object file does not define any previously undefined public symbols...".
        # Enable "Generate Debug Information" in release config by setting "/Zi" and "/DEBUG" flags.
        if(MSVC)
            target_compile_options(${target} 
                PRIVATE 
                    /wd4221
                    $<$<CONFIG:RELEASE>:/Zi>
            )
            set_property(TARGET ${target} 
                APPEND PROPERTY 
                    LINK_FLAGS_RELEASE /DEBUG
            )
        endif(MSVC)
    elseif(SMTG_LINUX)
        smtg_get_linux_architecture_name() # Sets var LINUX_ARCHITECTURE_NAME
        message(STATUS "[SMTG] Linux architecture name is ${LINUX_ARCHITECTURE_NAME}.")

        get_target_property(PLUGIN_PACKAGE_CONTENTS ${target} SMTG_PLUGIN_PACKAGE_CONTENTS)
        set_target_properties(${target}
            PROPERTIES
                PREFIX                   ""
                LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}/${PLUGIN_PACKAGE_CONTENTS}/${LINUX_ARCHITECTURE_NAME}
                SMTG_PLUGIN_PACKAGE_PATH ${PLUGIN_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PLUGIN_PACKAGE_NAME}
        )

        # Strip symbols in Release config
        if(${CMAKE_BUILD_TYPE} MATCHES Release)
            smtg_target_strip_symbols(${target})
        elseif(${CMAKE_BUILD_TYPE} MATCHES RelWithDebInfo)
            smtg_target_strip_symbols_with_dbg(${target})
        endif()
    endif(SMTG_MAC)

    smtg_target_create_resources_folder(${target})
endfunction(smtg_target_make_plugin_package)

function(smtg_target_set_vst_win_architecture_name target)
    if(SMTG_WIN)
        if(DEFINED CMAKE_GENERATOR_PLATFORM AND CMAKE_GENERATOR_PLATFORM)
            string(TOLOWER ${CMAKE_GENERATOR_PLATFORM} GENERATOR_PLATFORM)
        elseif(DEFINED CMAKE_MODULE_LINKER_FLAGS AND CMAKE_MODULE_LINKER_FLAGS)
            string(TOLOWER ${CMAKE_MODULE_LINKER_FLAGS} GENERATOR_PLATFORM)
        endif()

        if(${GENERATOR_PLATFORM} MATCHES "arm64ec")
            set(WIN_ARCHITECTURE_NAME "arm64ec")
        elseif(${GENERATOR_PLATFORM} MATCHES "arm64")
            set(WIN_ARCHITECTURE_NAME "arm64")
        elseif(${GENERATOR_PLATFORM} MATCHES "arm")
            set(WIN_ARCHITECTURE_NAME "arm")
        elseif(${GENERATOR_PLATFORM} MATCHES "win32")
            set(WIN_ARCHITECTURE_NAME "x86")
        else()
            set(WIN_ARCHITECTURE_NAME "x86_64")
        endif()
        set(WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}-win)

        set_target_properties(${target}
            PROPERTIES
                SMTG_WIN_ARCHITECTURE_NAME ${WIN_ARCHITECTURE_NAME}
        )
    endif(SMTG_WIN)
endfunction(smtg_target_set_vst_win_architecture_name)

function(smtg_add_vst3plugin_with_pkgname target pkg_name)
    #message(STATUS "[SMTG] target is ${target}")
    #message(STATUS "[SMTG] pkg_name is ${pkg_name}")

    set(oneValueArgs MODULEINFO_COMPATIBILITY)
    set(sourcesArgs SOURCES_LIST)
    cmake_parse_arguments(PARSE_ARGV 2 PARAMS "" "${oneValueArgs}" "${sourcesArgs}")

    #message(STATUS "[SMTG] PARAMS_UNPARSED_ARGUMENTS is ${PARAMS_UNPARSED_ARGUMENTS}")
    #message(STATUS "[SMTG] PARAMS_MODULEINFO_COMPATIBILITY is ${PARAMS_MODULEINFO_COMPATIBILITY}")
    #message(STATUS "[SMTG] PARAMS_SOURCES_LIST is ${PARAMS_SOURCES_LIST}")

    if(NOT EXISTS ${SMTG_PACKAGE_ICON_PATH})
        set(SMTG_PACKAGE_ICON_PATH ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/VST_Logo_Steinberg.ico)
    endif()
    #message(STATUS "[SMTG] SMTG_PACKAGE_ICON_PATH is ${SMTG_PACKAGE_ICON_PATH}")

    if(DEFINED PARAMS_MODULEINFO_COMPATIBILITY)
        set(SOURCES "${PARAMS_SOURCES_LIST}")
    else()
        set(SOURCES "${PARAMS_UNPARSED_ARGUMENTS}")
    endif()

    add_library(${target} MODULE "${SOURCES}")
    smtg_target_set_vst_win_architecture_name(${target})
    smtg_target_make_plugin_package(${target} ${pkg_name} vst3)

    if(SMTG_ENABLE_TARGET_VARS_LOG)
        smtg_target_dump_plugin_package_variables(${target})
    endif(SMTG_ENABLE_TARGET_VARS_LOG)

    if(DEFINED PARAMS_MODULEINFO_COMPATIBILITY)
        smtg_target_create_module_info_file(${target} MODULEINFO_COMPATIBILITY "${PARAMS_MODULEINFO_COMPATIBILITY}")
    else()
#         smtg_target_create_module_info_file(${target})
    endif()

    if(SMTG_RUN_VST_VALIDATOR)
        smtg_target_run_vst_validator(${target})
    endif(SMTG_RUN_VST_VALIDATOR)

    if(SMTG_CREATE_PLUGIN_LINK)
        smtg_get_vst3_path()
        smtg_target_create_link_to_plugin(${target} ${SMTG_PLUGIN_TARGET_PATH})
    endif(SMTG_CREATE_PLUGIN_LINK)

endfunction(smtg_add_vst3plugin_with_pkgname)

#------------------------------------------------------------------------
# Adds a VST3 target.
#
# @param target The target to which a VST3 Plug-in will be added. 
# @param PACKAGE_NAME <Name> The name of the package [optional] if not present
# @param MODULEINFO_COMPATIBILITY <path> The path to the compatibility json file [optional]
# @param SOURCES_LIST <sources> List of sources to add to target project [mandatory when PACKAGE_NAME is used]
# for example: 
# smtg_add_vst3plugin(${target} PACKAGE_NAME "A Gain" SOURCES_LIST ${again_sources})
# smtg_add_vst3plugin(${target} PACKAGE_NAME "A Gain" MODULEINFO_COMPATIBILITY compat.json SOURCES_LIST ${again_sources})
# or
# smtg_add_vst3plugin(${target} ${again_sources})
function(smtg_add_vst3plugin target)
    set(oneValueArgs PACKAGE_NAME MODULEINFO_COMPATIBILITY)
    set(sourcesArgs SOURCES_LIST)
    cmake_parse_arguments(PARSE_ARGV 1 PARAMS "${options}" "${oneValueArgs}" "${sourcesArgs}")

    #message(STATUS "[SMTG] PARAMS_UNPARSED_ARGUMENTS is ${PARAMS_UNPARSED_ARGUMENTS}")
    #message(STATUS "[SMTG] PARAMS_PACKAGE_NAME is ${PARAMS_PACKAGE_NAME}")
    #message(STATUS "[SMTG] PARAMS_SOURCES is ${PARAMS_SOURCES_NAME}")
    #message(STATUS "[SMTG] PARAMS_MODULEINFO_COMPATIBILITY is ${PARAMS_MODULEINFO_COMPATIBILITY}")

    set(SOURCES "${PARAMS_SOURCES_LIST}")
    if(SOURCES STREQUAL "")
        set(SOURCES ${PARAMS_UNPARSED_ARGUMENTS})
    endif()
   
    set(pkg_name "${PARAMS_PACKAGE_NAME}")
    if(pkg_name STREQUAL "")
        set(pkg_name ${target})
    endif()

    #message(STATUS "[SMTG] target is ${target}.")
    #message(STATUS "[SMTG] pkg_name is ${pkg_name}.")
    #message(STATUS "[SMTG] SOURCES is ${SOURCES}.")

    if(DEFINED PARAMS_MODULEINFO_COMPATIBILITY)
        set(SMTG_MODULEINFO_COMPATIBILITY "${PARAMS_MODULEINFO_COMPATIBILITY}")
        smtg_add_vst3plugin_with_pkgname(${target} ${pkg_name} MODULEINFO_COMPATIBILITY "${SMTG_MODULEINFO_COMPATIBILITY}" SOURCES_LIST ${SOURCES})
    else()
        smtg_add_vst3plugin_with_pkgname(${target} ${pkg_name} ${SOURCES})
    endif()
endfunction(smtg_add_vst3plugin)


if (WIN32)
list(APPEND HEADER_FILES "windows/editor_view.hpp")
smtg_add_vst3plugin(VIE_AUV3
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
    "assets/config/desktop.ini"
    "assets/config/default_instrument.json"
    "assets/config/vie.ico"
    "assets/view/"
    "windows/application.cpp"
    "windows/editor_view.cpp"
)
elseif(APPLE)
smtg_add_vst3plugin(VIE_AUV3
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
    "assets/config/desktop.ini"
    "assets/config/default_instrument.json"
    "assets/config/vie.ico"
    "assets/view/"
    "ios/editor_view.hpp"
    "ios/application.cpp"
    "ios/editor_view.cpp"
)
endif ()


# add_library(VIE_AUV3 MODULE "${SOURCES}")

add_dependencies(VIE_AUV3
    vst_interfaces
    cycfi_q
    libsndfile
)

smtg_target_add_plugin_snapshots (VIE_AUV3
    RESOURCES
        resource/4A02F0BEF6E35C0AA99B7F79286D83B9_snapshot.png
        resource/4A02F0BEF6E35C0AA99B7F79286D83B9_snapshot_2.0x.png
)

set(JSON_DIRECTORY "../third_party/nlohman_json")
target_include_directories(VIE_AUV3 PUBLIC ${JSON_DIRECTORY})

target_link_libraries(VIE_AUV3
    PRIVATE
#         sdk
        vst_interfaces
        cycfi_q
        sndfile
)

smtg_target_configure_version_file(VIE_AUV3)

smtg_target_set_bundle(VIE_AUV3
    BUNDLE_IDENTIFIER live.tritone.vie
    COMPANY_NAME "TriTone"
)

smtg_target_set_debug_executable(VIE_AUV3
    "/Applications/VST3PluginTestHost.app"
    "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
)