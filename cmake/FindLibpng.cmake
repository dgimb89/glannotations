#  LIBPNG_FOUND
#  LIBPNG_LIBRARIES

#  LIBPNG_INCLUDE_DIR 
#  LIBPNG_LIBRARY	
#  LIBPNG_LIBRARY_DEBUG

find_path(	LIBPNG_INCLUDE_DIR
	NAMES 
		png.h
	HINTS
		$ENV{LIBPNG_HOME}
		${LIBPNG_INCLUDE_DIR}
		${NSCP_INCLUDEDIR}
		/usr
	PATH_SUFFIXES 
		include
)

find_library(	LIBPNG_LIBRARY 
	NAMES 
		libpng
		libpng16
		png
	PATHS
		$ENV{LIBPNG_HOME}/lib
		/usr/lib/x86_64-linux-gnu
		/usr/lib
)



find_library( LIBPNG_LIBRARY_DEBUG
	NAMES 
		libpngd
		libpng16d
	PATHS
		$ENV{LIBPNG_HOME}/lib
		/usr/lib/x86_64-linux-gnu
		/usr/lib
)

set(LIBPNG_LIBRARIES "optimized" ${LIBPNG_LIBRARY} "debug" ${LIBPNG_LIBRARY_DEBUG})

if(NOT LIBPNG_LIBRARY AND LIBPNG_LIBRARY_DEBUG)
	message(STATUS "LibPNG Release version not found; using debug")
	set(LIBPNG_LIBRARIES ${LIBPNG_LIBRARY_DEBUG})
elseif(LIBPNG_LIBRARY AND NOT LIBPNG_LIBRARY_DEBUG)
	message(STATUS "LibPNG Debug version not found; using release")
	set(LIBPNG_LIBRARIES ${LIBPNG_LIBRARY})
endif()

### Debug ###
message(STATUS "LibPNG: " ${LIBPNG_LIBRARIES})
message(STATUS "LibPNG: " ${LIBPNG_INCLUDE_DIR})

if(LIBPNG_LIBRARIES AND LIBPNG_INCLUDE_DIR)
	set(LIBPNG_FOUND TRUE)
else(LIBPNG_LIBRARIES AND LIBPNG_INCLUDE_DIR)
	set(LIBPNG_FOUND FALSE)
	message(WARNING "Note: Environment variable LIBPNG_HOME assists this script to locate LIBPNG")
endif(LIBPNG_LIBRARIES AND LIBPNG_INCLUDE_DIR)

mark_as_advanced(LIBPNG_INCLUDE_DIR)