cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

# PROJECT DESCRIPTION
set(META_PROJECT_NAME        "glat")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_DOMAIN       "https://github.com/dgimb89/glat")

# Project
project(${META_PROJECT_NAME})


# OPTIONS
option(OPTION_USE_NVPR "Use NV Path Rendering" ON)
option(OPTION_BUILD_EXAMPLES "Build examples" OFF)
option(OPTION_BUILD_STATIC "Build static" OFF)


# CMAKE CONFIGURATION
set(GLAT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "${GLAT_DIR}")

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${GLAT_DIR}/cmake/Modules/)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(RESOURCES_DIR "${CMAKE_BINARY_DIR}/resources/" CACHE FILEPATH "GLAT")


# Generate folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "d")

# Include custom cmake functions
include(cmake/common.cmake)


# GLAT DEPENDENCIES

# Find externals
find_package(OpenGL REQUIRED)
find_package(GLOW REQUIRED)
find_package(Libpng REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)

SET(RAPIDJSON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/rapidjson/include)

# Include directories
include_directories(
	${GLEW_INCLUDE_DIR}
    ${GLAT_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLOW_INCLUDE_DIR}
    ${LIBPNG_INCLUDE_DIR})

# LIBRARIES
add_subdirectory(src)

# RESOURCES
file(COPY "resources" DESTINATION .)

# EXAMPLES
if(OPTION_BUILD_EXAMPLES)
	
	find_package(GLFW REQUIRED glfw3)
	SET(GLAT_EXAMPLES_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/examples/)
    add_subdirectory(examples)
endif()

# DEPLOYMENT
install(DIRECTORY ${GLAT_DIR}/include/ DESTINATION include/)
