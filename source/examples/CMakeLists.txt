if(OPTION_BUILD_EXAMPLES)

	get_property(GENERATED_HEADER_PATH GLOBAL PROPERTY GENERATED_HEADER_PATH_PROPERTY)

	set(EXAMPLE_COMMON examplecommon)
	add_subdirectory("common")
	include_directories("common")

	add_subdirectory(df-rendering)
	add_subdirectory(nvp-rendering)
	add_subdirectory(presentation-scene)

	if (OPTION_BUILD_GLAT_PREPROCESSOR)
		add_subdirectory(font-preprocessing)
	endif()
endif()