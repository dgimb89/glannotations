
set(target ${EXAMPLE_COMMON})
message(STATUS "Helper ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLFW REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)

#
# Includes
#

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
)

get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/examples
    ${GENERATED_HEADER_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARY}
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
)

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(api_includes
    ${include_path}/AbstractCoordinateProvider.h
    ${include_path}/AxisAlignedBoundingBox.h
    ${include_path}/Camera.h
    ${include_path}/Context.h
    ${include_path}/ContextFormat.h
    ${include_path}/events.h
    ${include_path}/MainLoop.h
    ${include_path}/navigationmath.h
    ${include_path}/Timer.h
    ${include_path}/Window.h
    ${include_path}/WindowEventDispatcher.h
    ${include_path}/WindowEventHandler.h
    ${include_path}/WorldInHandNavigation.h
)

set(sources
    ${source_path}/AbstractCoordinateProvider.cpp
    ${source_path}/AxisAlignedBoundingBox.cpp
    ${source_path}/Camera.cpp
    ${source_path}/Context.cpp
    ${source_path}/ContextFormat.cpp
    ${source_path}/events.cpp
    ${source_path}/MainLoop.cpp
    ${source_path}/navigationmath.cpp
    ${source_path}/Timer.cpp
    ${source_path}/Window.cpp
    ${source_path}/WindowEventDispatcher.cpp
    ${source_path}/WindowEventHandler.cpp
    ${source_path}/WorldInHandNavigation.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

add_library(${target} STATIC ${api_includes} ${sources})
target_link_libraries(${target} ${libs})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})
