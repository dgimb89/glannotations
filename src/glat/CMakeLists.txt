set(TARGET glat)

# Sources
set(header_path "${GLAT_DIR}/include/glat")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/AbstractRenderMethod.h
	${header_path}/AbstractState.h
	${header_path}/Annotation.h
	${header_path}/RenderMethodFactory.h
	${header_path}/Styling.h
)

set(sources
	${source_path}/Annotation.cpp
	${source_path}/Styling.cpp
	${source_path}/RenderMethodFactory.cpp
)

# Group source files
set(glat_header_group "Header Files")
set(glat_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glat_header_group} ${header})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glat_source_group} ${sources})

# Build library
if(OPTION_BUILD_STATIC)
    add_library(${TARGET} ${header} ${sources})
else()
    add_library(${TARGET} SHARED ${header} ${sources})
endif()

target_link_libraries(${TARGET}
	${OPENGL_LIBRARIES}
)