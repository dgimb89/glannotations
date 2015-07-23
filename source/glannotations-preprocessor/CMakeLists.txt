
set(target glannotations-preprocessor)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(Freetype REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(GLM REQUIRED)

#
# Includes
#

include_directories(
    ${FREETYPE_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glannotations/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
    glannotations
    ${FREETYPE_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLANNOTATIONS_STATIC")
else()
    add_definitions("-DGLANNOTATIONS_PREPROCESSOR_EXPORTS")
endif()

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(includes
    ${include_path}/glannotations-preprocessor_api.h
    ${include_path}/GlyphSetGenerator.h
)

set(sources
	${source_path}/GlyphSetGenerator.cpp
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
    INCLUDE_PATH                ${include_path})

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
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
