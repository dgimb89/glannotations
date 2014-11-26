
set(target osg-wrapper)
message(STATUS "Example ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
find_package(GLOBJECTS REQUIRED)
find_package(GLBinding REQUIRED)
find_package(OpenSceneGraph REQUIRED)
find_package(osgViewer REQUIRED)
find_package(osgGA REQUIRED)
find_package(osgDB REQUIRED)

#
# Includes
#

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLOBJECTS_INCLUDES}
    ${GLBINDING_INCLUDES}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${OSGVIEWER_INCLUDE_DIRS}
    ${OSGGA_INCLUDE_DIRS}
    ${OSGDB_INCLUDE_DIRS}
)

include_directories(
	BEFORE
    ${CMAKE_SOURCE_DIR}/source/glannotations/include
    ${CMAKE_SOURCE_DIR}/source/glannotations-wrapper/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# Libraries
#

set(libs
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARY}
    ${GLOBJECTS_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
    ${OSGVIEWER_LIBRARIES}
    ${OSGGA_LIBRARIES}
    ${OSGDB_LIBRARIES}
    glannotations
    glannotations-wrapper
)

#
# Sources
#

set(sources
    main.cpp
)

#
# Build executable
#

add_executable(${target} ${sources})

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
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}")

#
# Deployment
#

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
#   LIBRARY DESTINATION ${INSTALL_SHARED}
#   ARCHIVE DESTINATION ${INSTALL_LIB}
)
