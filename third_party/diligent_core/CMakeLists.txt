message(STATUS "VIE> DiligentCore> Fetch")

include(FetchContent)

FetchContent_Declare(
    diligent_core
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentCore.git
    GIT_TAG        v2.5.1
)

FetchContent_GetProperties(diligent_core)
#if(NOT diligent_core_POPULATED)
#	FetchContent_Populate(diligent_core)
#endif()

FetchContent_MakeAvailable(diligent_core)

message(STATUS "VIE> DiligentCore> Add library")
add_library(diligent_core INTERFACE)

message(STATUS "VIE> DiligentCore> Include directory: " ${diligent_core_SOURCE_DIR})
#set(DILIGENT_NO_DIRECT3D11 OFF CACHE BOOL "TRUE")
#set(DILIGENT_NO_DIRECT3D12 OFF CACHE BOOL "TRUE")
#target_compile_options(diligent_core INTERFACE -DUNICODE -DENGINE_DLL)
target_compile_options(diligent_core INTERFACE -DUNICODE)
target_include_directories(diligent_core INTERFACE ${diligent_core_SOURCE_DIR})

message(STATUS "VIE> DiligentCore> Link directory: " ${diligent_core_BINARY_DIR})
target_link_directories(diligent_core INTERFACE ${diligent_core_BINARY_DIR})
