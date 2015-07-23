find_package(OpenSceneGraph REQUIRED)
if (NOT OSG_FOUND)
    message(WARNING "glannotations osg-wrapper disabled: No OSG found")
    
    return ()
endif ()

set(target glannotations-wrapper)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(gloperate REQUIRED)
find_package(libzeug REQUIRED)

#
# Libraries
#

set(libs
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${GLOPERATE_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
	glannotations
)

#
# Includes
#
include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glannotations/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${GLOPERATE_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLANNOTATIONS_STATIC")
else()
    add_definitions("-DGLANNOTATIONS_WRAPPER_EXPORTS")
endif()

# force radians to avoid deprecated glm functionality
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(includes
    ${include_path}/glannotations-wrapper_api.h
    ${include_path}/OSGAnnotationDrawable.h
)

set(sources
	${source_path}/OSGAnnotationDrawable.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

add_library(${target} ${includes} ${sources})

target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})

#
# Deployment
#

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${include_path} DESTINATION ${INSTALL_INCLUDE})
