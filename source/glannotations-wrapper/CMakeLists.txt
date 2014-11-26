
set(target glannotations-wrapper)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenSceneGraph REQUIRED)
find_package(GLOBJECTS REQUIRED)
find_package(GLBinding REQUIRED)

#
# Libraries
#


include_directories(
    BEFORE
    ${GLBINDING_INCLUDES}
    ${CMAKE_SOURCE_DIR}/source/glannotations/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLOBJECTS_INCLUDES}
    ${GLM_INCLUDE_DIR}
)

set(libs
	glannotations
)

#
# Includes
#
if(OSG_FOUND)
	include_directories(
		${OPENSCENEGRAPH_INCLUDE_DIRS}
	)
	set(libs
		${libs}
		${OPENSCENEGRAPH_LIBRARIES}
    	${GLOBJECTS_LIBRARIES}
	)
endif()

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLANNOTATIONS_WRAPPER_STATIC")
else()
    add_definitions("-DGLANNOTATIONS_WRAPPER_EXPORTS")
endif()

# force radians to avoid deprecated glm functionality
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/glannotations/wrapper")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
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
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})

#
# Build library
#

if(OPTION_BUILD_STATIC)
    add_library(${target} ${api_includes} ${sources})
else()
    add_library(${target} SHARED ${api_includes} ${sources})
endif()

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
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/glannotations DESTINATION ${INSTALL_INCLUDE})
