if(OPTION_BUILD_EXAMPLES)

	get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

	set(EXAMPLE_COMMON common)
	add_subdirectory("common")
	include_directories("common")

	add_subdirectory(rendering-example)

	if (OPTION_BUILD_GLAT_PREPROCESSOR)
		add_subdirectory(font-preprocessing)
	endif()

	if (OPTION_BUILD_GLAT_WRAPPER)
		add_subdirectory(osg-wrapper)
	endif()
endif()