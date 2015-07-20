if(OPTION_BUILD_EXAMPLES)

	get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

	set(EXAMPLE_COMMON common)
	add_subdirectory("common")
	include_directories("common")

	add_subdirectory(rendering-example)
	add_subdirectory(annotation-space)

	if (OPTION_BUILD_GLANNOTATIONS_PREPROCESSOR)
		add_subdirectory(font-preprocessing)
	endif()

	if (OPTION_BUILD_GLANNOTATIONS_WRAPPER)
		add_subdirectory(osg-wrapper)
	endif()
endif()