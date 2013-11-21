set(TARGET glat)

# Sources
set(header_path "${GLAT_DIR}/include/glat")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/AbstractRenderMethod.h
	${header_path}/NVPathRendering.h
	${header_path}/DistanceFieldMapping.h
	${header_path}/AbstractState.h
	${header_path}/AbstractAnnotation.h
	${header_path}/RenderMethodFactory.h
	${header_path}/Styling.h
)

set(sources
	${source_path}/AbstractAnnotation.cpp
	${source_path}/Styling.cpp
	${source_path}/RenderMethodFactory.cpp
	${source_path}/AbstractRenderMethod.cpp
	${source_path}/NVPathRendering.cpp
	${source_path}/DistanceFieldMapping.cpp
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
	${OPENGL_LIBRARIES}
)