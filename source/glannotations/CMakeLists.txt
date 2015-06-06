
set(target glannotations)
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
find_package(Libpng REQUIRED)

#
# Includes
#

include_directories(
    ${LIBPNG_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLBINDING_INCLUDES}
    ${GLOBJECTS_INCLUDES}
    ${GLOPERATE_INCLUDES}
    ${LIBZEUG_INCLUDES}
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
    ${GLBINDING_LIBRARIES}
    ${GLOBJECTS_LIBRARIES}
    ${GLOPERATE_LIBRARIES}
    ${LIBZEUG_LIBRARIES}
)

#
# Compiler definitions
#

if (OPTION_BUILD_STATIC)
    add_definitions("-DGLANNOTATIONS_STATIC")
else()
    add_definitions("-DGLANNOTATIONS_EXPORTS")
endif()

# force radians to avoid deprecated glm functionality
add_definitions("-DGLM_FORCE_RADIANS")

#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/glannotations_api.h
    ${include_path}/AbstractAnnotation.h
    ${include_path}/AnnotationGroup.h
    ${include_path}/FontAnnotation.h
    ${include_path}/PNGAnnotation.h
    ${include_path}/SVGAnnotation.h

    ${include_path}/Common/DirtyFlagObject.h
	${include_path}/Common/globals.h
    ${include_path}/Common/GLRawFile.h
    ${include_path}/Common/PNGImage.h
    ${include_path}/Common/TextureManager.h

    ${include_path}/Externals/AbstractExternalReference.h
    ${include_path}/Externals/BoxReference.h
    ${include_path}/Externals/FlagReference.h
    ${include_path}/Externals/LabelReference.h
    ${include_path}/Externals/RectReference.h

    ${include_path}/Renderer/AbstractDrawingPrimitive.h
    ${include_path}/Renderer/AbstractPrimitiveRenderer.h
    ${include_path}/Renderer/AbstractRenderer.h
    ${include_path}/Renderer/AbstractTexturedPrimitive.h
    ${include_path}/Renderer/BendedQuadStrip.h
    ${include_path}/Renderer/Box.h
    ${include_path}/Renderer/DistanceFieldFontRenderer.h
    ${include_path}/Renderer/DistanceFieldPNGRenderer.h
    ${include_path}/Renderer/DrawingPrimitiveBase.h
    ${include_path}/Renderer/NVPRFontRenderer.h
    ${include_path}/Renderer/NVPRRenderer.h
    ${include_path}/Renderer/NVPRSvgRenderer.h
    ${include_path}/Renderer/Prismoid.h
    ${include_path}/Renderer/QuadStrip.h
    ${include_path}/Renderer/Rect.h
    ${include_path}/Renderer/RendererFactory.h

    ${include_path}/States/AbstractState.h
    ${include_path}/States/InternalState.h
    ${include_path}/States/ReferenceableState.h
    ${include_path}/States/SplineState.h
    ${include_path}/States/StateInterpolation.h
    ${include_path}/States/ViewportState.h

    ${include_path}/Styles/BumpMap.h
    ${include_path}/Styles/ExternalColor.h
    ${include_path}/Styles/Outline.h
    ${include_path}/Styles/Styles.h
	${include_path}/Styles/Styling.h

    ${include_path}/Utility/BoundingBox.h
    ${include_path}/Utility/BSpline.h
    ${include_path}/Utility/DistanceFieldGeneration.h
    ${include_path}/Utility/GlyphSetConfig.h
	${include_path}/Utility/Utility.h
	${include_path}/Utility/Segment.h
	${include_path}/Utility/Triangle.h
	${include_path}/Utility/Plane.h
)

set(sources
    ${source_path}/AbstractAnnotation.cpp
    ${source_path}/AnnotationGroup.cpp
    ${source_path}/FontAnnotation.cpp
    ${source_path}/PNGAnnotation.cpp
    ${source_path}/ShaderSources.hpp
    ${source_path}/SVGAnnotation.cpp

    ${source_path}/Common/DirtyFlagObject.cpp
    ${source_path}/Common/globals.cpp
    ${source_path}/Common/GLRawFile.cpp
    ${source_path}/Common/PNGImage.cpp
    ${source_path}/Common/TextureManager.cpp

    ${source_path}/Externals/AbstractExternalReference.cpp
    ${source_path}/Externals/BoxReference.cpp
    ${source_path}/Externals/FlagReference.cpp
    ${source_path}/Externals/LabelReference.cpp
    ${source_path}/Externals/RectReference.cpp

    ${source_path}/Renderer/AbstractPrimitiveRenderer.cpp
    ${source_path}/Renderer/AbstractRenderer.cpp
    ${source_path}/Renderer/AbstractTexturedPrimitive.cpp
    ${source_path}/Renderer/BendedQuadStrip.cpp
    ${source_path}/Renderer/Box.cpp
    ${source_path}/Renderer/DistanceFieldFontRenderer.cpp
    ${source_path}/Renderer/DistanceFieldPNGRenderer.cpp
    ${source_path}/Renderer/DrawingPrimitiveBase.cpp
    ${source_path}/Renderer/NVPRFontRenderer.cpp
    ${source_path}/Renderer/NVPRRenderer.cpp
    ${source_path}/Renderer/NVPRSvgRenderer.cpp
    ${source_path}/Renderer/Prismoid.cpp
    ${source_path}/Renderer/QuadStrip.cpp
    ${source_path}/Renderer/Rect.cpp
    ${source_path}/Renderer/RendererFactory.cpp

    ${source_path}/States/AbstractState.cpp
    ${source_path}/States/InternalState.cpp
    ${source_path}/States/ReferenceableState.cpp
    ${source_path}/States/SplineState.cpp
    ${source_path}/States/StateInterpolation.cpp
    ${source_path}/States/ViewportState.cpp

    ${source_path}/Styles/BumpMap.cpp
    ${source_path}/Styles/ExternalColor.cpp
    ${source_path}/Styles/Outline.cpp
    ${source_path}/Styles/Styling.cpp

    ${source_path}/Utility/BoundingBox.cpp
    ${source_path}/Utility/BSpline.cpp
    ${source_path}/Utility/DistanceFieldGeneration.cpp
    ${source_path}/Utility/GlyphSetConfig.cpp
    ${source_path}/Utility/Utility.cpp
    ${source_path}/Utility/Segment.cpp
    ${source_path}/Utility/Triangle.cpp
    ${source_path}/Utility/Plane.cpp
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
configure_file(${source_path}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
