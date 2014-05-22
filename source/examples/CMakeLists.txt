if(OPTION_BUILD_EXAMPLES)
	if(WIN32)
		install(FILES ${GLEW_INCLUDE_DIR}/../bin/glew32.dll DESTINATION ${INSTALL_BIN})
	endif()

	include_directories(
	    ${GLOW_INCLUDES}
	    ${GLM_INCLUDE_DIR}
	)

	include_directories(
	    BEFORE
	    ${CMAKE_SOURCE_DIR}/source/glat/include
	    ${CMAKE_SOURCE_DIR}/source/glat_preprocessor/include
	)

	get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

	set(EXAMPLE_COMMON examplecommon)
	add_subdirectory("common")
	include_directories("common")

	add_subdirectory(DFRendering)
	add_subdirectory(NVPRendering)
	#add_subdirectory(PresentationScene)

	if (OPTION_BUILD_GLAT_PREPROCESSOR)
		#add_subdirectory(DFPreprocessing)
	endif()
endif()