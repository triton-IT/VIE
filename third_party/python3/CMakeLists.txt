message(STATUS "VIE> python3> Find")

include(FindPython3)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

message(STATUS "VIE> python3> Add library")
add_library(python3 INTERFACE)

message(STATUS "VIE> python3> Include directories: ${Python3_INCLUDE_DIRS}")
target_include_directories(python3 INTERFACE ${Python3_INCLUDE_DIRS})

message(STATUS "VIE> python3> Link directory: " ${Python3_LIBRARY_DIRS})
target_link_directories(python3 INTERFACE ${Python3_LIBRARY_DIRS})

#FIXME: Debug only if DEBUG is enabled.
target_link_libraries(python3 ${Python3_DEBUG_LIBRARIES})