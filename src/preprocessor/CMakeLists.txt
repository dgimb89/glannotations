set(TARGET preprocessor)

# Sources
set(header_path "${GLAT_DIR}/include/glat/preprocessor")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/GlyphSetGenerator.h
)

set(libsources
	${source_path}/GlyphSetGenerator.cpp
)

set(sources
	${source_path}
)

# Group source files
set(glat_header_group "Header Files")
set(glat_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glat_header_group} ${header})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glat_source_group} ${sources})


include_directories(
		${FREETYPE_INCLUDE_DIR_ft2build}
		${FREETYPE_INCLUDE_DIR_freetype2}
)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories (${CMAKE_CURRENT_BINARY_DIR}) 

# build library
if(OPTION_BUILD_STATIC)
    add_library(${TARGET} ${header} ${libsources})
else()
    add_library(${TARGET} SHARED ${header} ${libsources})
endif()

target_link_libraries(${TARGET}
	glat
	${FREETYPE_LIBRARY}
	${OPENGL_LIBRARIES}
)