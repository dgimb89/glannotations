set(TARGET glat)

# Sources
set(header_path "${GLAT_DIR}/include/glat")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/Object.h
	${header_path}/AbstractAnnotation.h
	${header_path}/FontAnnotation.h
	${header_path}/SVGAnnotation.h
	${header_path}/PDFAnnotation.h

	${header_path}/Styling.h
	${header_path}/Outline.h
	${header_path}/BumpMap.h

	${header_path}/RendererFactory.h
	${header_path}/AbstractRenderer.h

	${header_path}/NVPRRenderer.h
	${header_path}/NVPRFontRenderer.h
	${header_path}/NVPRSvgRenderer.h

	${header_path}/DistanceFieldRenderer.h

	${header_path}/AbstractState.h
	${header_path}/ViewportState.h
	
	${header_path}/Quad.h
)

set(sources
	${source_path}/Object.cpp
	${source_path}/AbstractAnnotation.cpp
	${source_path}/FontAnnotation.cpp
	${source_path}/SVGAnnotation.cpp
	${source_path}/PDFAnnotation.cpp

	${source_path}/Styling.cpp
	${source_path}/Outline.cpp
	${source_path}/BumpMap.cpp

	${source_path}/RendererFactory.cpp

	${source_path}/NVPRRenderer.cpp
	${source_path}/NVPRFontRenderer.cpp
	${source_path}/NVPRSvgRenderer.cpp

	${source_path}/DistanceFieldRenderer.cpp

	${source_path}/AbstractState.cpp
	${source_path}/ViewportState.cpp
	${source_path}/Quad.cpp

	${source_path}/nvpr_init.h
	${source_path}/nvpr_init.c
	
	${source_path}/ShaderSources.hpp
)

# Group source files
set(glat_header_group "Header Files")
set(glat_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glat_header_group} ${header})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glat_source_group} ${sources})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories (${CMAKE_CURRENT_BINARY_DIR}) 

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${TARGET} ${header} ${sources})
else()
    add_library(${TARGET} SHARED ${header} ${sources})
endif()

target_link_libraries(${TARGET}
	${GLEW_LIBRARY}
	${OPENGL_LIBRARIES}
	${GLOW_LIBRARIES}
	${LIBPNG_LIBRARY}
)