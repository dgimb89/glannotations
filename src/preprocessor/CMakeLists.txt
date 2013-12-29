set(TARGET preprocessor)

# Sources
set(header_path "${GLAT_DIR}/include/glat/preprocessor")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(header
	${header_path}/SVGLoader.h
	${header_path}/DistanceFieldGenerator.h
)

set(sources
	${source_path}/main.cpp
	${source_path}/SVGLoader.cpp
	${source_path}/DistanceFieldGenerator.cpp
)

# Group source files
set(glat_header_group "Header Files")
set(glat_source_group "Source Files")
source_group_by_path(${header_path} "\\\\.h$|\\\\.hpp$" 
    ${glat_header_group} ${header})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${glat_source_group} ${sources})

# Build library
#if(OPTION_BUILD_STATIC)
#    add_library(${TARGET} ${header} ${sources})
#else()
#    add_library(${TARGET} SHARED ${header} ${sources})
#endif()

find_package(TinyXML2 REQUIRED)

include_directories(	
		${TINYXML2_INCLUDE_DIR}
)

# Build executable
add_executable(${TARGET} ${header} ${sources})

#add_subdirectory(libsvg)

target_link_libraries(${TARGET}
	${OPENGL_LIBRARIES}
	${TINYXML2_LIBRARY}
	#libsvg
)