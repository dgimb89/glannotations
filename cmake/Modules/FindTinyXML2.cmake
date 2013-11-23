#  TINYXML2_FOUND
#  TINYXML2_INCLUDE_DIR 
#  TINYXML2_LIBRARY	

find_path(	TINYXML2_INCLUDE_DIR
	NAMES 
		tinyxml2.h
	HINTS
		$ENV{TINYXML2_HOME}
		${TINYXML2_INCLUDE_DIR}
		${NSCP_INCLUDEDIR}
	PATH_SUFFIXES 
		include		
)

find_library( TINYXML2_LIBRARY 
	NAMES 
		tinyxml2
	PATHS
		$ENV{TINYXML2_HOME}/lib
)


if(TINYXML2_LIBRARY AND TINYXML2_INCLUDE_DIR)
	set(TINYXML2_FOUND TRUE)
else(TINYXML2_LIBRARY AND TINYXML2_INCLUDE_DIR)
	set(TINYXML2_FOUND FALSE)
	message(WARNING "Note: Environment variable TINYXML2_HOME assists this script to locate tinyxml2")
endif(TINYXML2_LIBRARY AND TINYXML2_INCLUDE_DIR)

mark_as_advanced(TINYXML2_INCLUDE_DIR)