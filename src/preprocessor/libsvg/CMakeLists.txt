set(TARGET libsvg)

# Sources
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/svg.h
	${header_path}/svg_ascii.h
	${header_path}/svg_hash.h
	${header_path}/svg_version.h
	${header_path}/svgint.h
)

set(sources
	${source_path}/svg.c
	${source_path}/svg_ascii.c
	${source_path}/svg_attribute.c
	${source_path}/svg_color.c
	${source_path}/svg_element.c
	${source_path}/svg_gradient.c
	${source_path}/svg_group.c
	${source_path}/svg_hash.c
	${source_path}/svg_image.c
	${source_path}/svg_length.c
	${source_path}/svg_paint.c
	${source_path}/svg_parser.c
	${source_path}/svg_parser_expat.c
	${source_path}/svg_parser_libxml.c
	${source_path}/svg_path.c
	${source_path}/svg_pattern.c
	${source_path}/svg_str.c
	${source_path}/svg_style.c
	${source_path}/svg_text.c
	${source_path}/svg_transform.c
)

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${TARGET} ${header} ${sources})
else()
    add_library(${TARGET} SHARED ${header} ${sources})
endif()

#find_package(TinyXML2 REQUIRED)

#include_directories(	
#		${TINYXML2_INCLUDE_DIR}
#)

# Build executable
#add_executable(${TARGET} ${header} ${sources})

#target_link_libraries(${TARGET}
#	${OPENGL_LIBRARIES}
#	${TINYXML2_LIBRARY}
#)