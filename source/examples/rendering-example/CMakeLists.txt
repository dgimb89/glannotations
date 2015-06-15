
set(target rendering-example)
message(STATUS "Example ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(globjects REQUIRED)
find_package(gloperate REQUIRED)
find_package(libzeug REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

add_definitions("-DGLM_FORCE_RADIANS")

#
# Includes
#

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIRS}
    ${GLOBJECTS_INCLUDE_DIRS}
    ${GLOPERATE_INCLUDE_DIRS}
    ${LIBZEUG_INCLUDE_DIRS}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/source/glannotations/include
    ${CMAKE_SOURCE_DIR}/source/examples
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
    ${GLOPERATE_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
    ${EXAMPLE_COMMON}
    glannotations
)

#
# Sources
#

set(sources
    building.h
    building.cpp
    main.cpp
)

#
# Build executable
#

add_executable(${target} ${sources})

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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

#
# Deployment
#

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
