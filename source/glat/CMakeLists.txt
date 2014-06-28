
set(target glat)
message(STATUS "Lib ${target}")

#
# External libraries
#

find_package(OpenGL REQUIRED)
find_package(GLOW REQUIRED)
find_package(GLBinding REQUIRED)
find_package(Libpng REQUIRED)
find_package(GLM REQUIRED)

#
# Includes
#

include_directories(
    ${LIBPNG_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLOW_INCLUDES}
    ${GLBINDING_INCLUDES}
)

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}/3rdparty/rapidjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#
# Libraries
#

set(libs
	${LIBPNG_LIBRARIES}
	${OPENGL_LIBRARY}
    ${GLOW_LIBRARIES}
    ${GLBINDING_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLAT_STATIC")
else()
    add_definitions("-DGLAT_EXPORTS")
endif()

# force radians to avoid deprecated glm functionality
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/glat_api.h
	${include_path}/globals.h

	${include_path}/DirtyFlagObject.h
	${include_path}/GlyphSetConfig.h

	${include_path}/AnnotationGroup.h
	${include_path}/AbstractAnnotation.h
	${include_path}/FontAnnotation.h
	${include_path}/SVGAnnotation.h
	${include_path}/PNGAnnotation.h

	${include_path}/Styling.h
	${include_path}/Styles.h
	${include_path}/Styles/Outline.h
	${include_path}/Styles/BumpMap.h
	${include_path}/Styles/ExternalColor.h

	${include_path}/TextureManager.h
	${include_path}/RendererFactory.h
	${include_path}/AbstractRenderer.h

	${include_path}/NVPRRenderer.h
	${include_path}/NVPRFontRenderer.h
	${include_path}/NVPRSvgRenderer.h

	${include_path}/PNGImage.h
	${include_path}/AbstractPrimitiveRenderer.h
	${include_path}/DistanceFieldGeneration.h
	${include_path}/DistanceFieldPNGRenderer.h
	${include_path}/DistanceFieldFontRenderer.h

	${include_path}/AbstractState.h
	${include_path}/AbstractExternalReference.h
	${include_path}/ReferenceableState.h
	${include_path}/ViewportState.h
	${include_path}/InternalState.h
	${include_path}/PathState.h
	${include_path}/BoxReference.h
	${include_path}/LabelReference.h
	${include_path}/BSpline.h
	${include_path}/StateInterpolation.h
	
	${include_path}/AbstractDrawingPrimitive.h
	${include_path}/Prismoid.h
	${include_path}/Box.h
	${include_path}/AbstractTexturedPrimitive.h
	${include_path}/QuadStrip.h

	${include_path}/Utility/Utility.h
	${include_path}/Utility/Segment.h
	${include_path}/Utility/Triangle.h
	${include_path}/Utility/Plane.h
)

set(sources
	${source_path}/globals.cpp

	${source_path}/DirtyFlagObject.cpp
	${source_path}/GlyphSetConfig.cpp

	${source_path}/AnnotationGroup.cpp
	${source_path}/AbstractAnnotation.cpp
	${source_path}/FontAnnotation.cpp
	${source_path}/SVGAnnotation.cpp
	${source_path}/PNGAnnotation.cpp

	${source_path}/Styling.cpp
	${source_path}/Styles/Outline.cpp
	${source_path}/Styles/BumpMap.cpp
	${source_path}/Styles/ExternalColor.cpp

	${source_path}/TextureManager.cpp
	${source_path}/RendererFactory.cpp
	${source_path}/AbstractRenderer.cpp

	${source_path}/NVPRRenderer.cpp
	${source_path}/NVPRFontRenderer.cpp
	${source_path}/NVPRSvgRenderer.cpp

	${source_path}/PNGImage.cpp
	${source_path}/AbstractPrimitiveRenderer.cpp
	${source_path}/DistanceFieldGeneration.cpp
	${source_path}/DistanceFieldPNGRenderer.cpp	
	${source_path}/DistanceFieldFontRenderer.cpp

	${source_path}/AbstractState.cpp
	${source_path}/AbstractExternalReference.cpp
	${source_path}/ReferenceableState.cpp
	${source_path}/ViewportState.cpp
	${source_path}/InternalState.cpp
	${source_path}/PathState.cpp
	${source_path}/BoxReference.cpp
	${source_path}/LabelReference.cpp
	${source_path}/BSpline.cpp
	${source_path}/StateInterpolation.cpp
	
	${source_path}/AbstractDrawingPrimitive.cpp
	${source_path}/Prismoid.cpp
	${source_path}/Box.cpp
	${source_path}/AbstractTexturedPrimitive.cpp
	${source_path}/QuadStrip.cpp
	
	${source_path}/Utility/Utility.cpp
	${source_path}/Utility/Segment.cpp
	${source_path}/Utility/Triangle.cpp
	${source_path}/Utility/Plane.cpp

	${source_path}/ShaderSources.hpp
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
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
