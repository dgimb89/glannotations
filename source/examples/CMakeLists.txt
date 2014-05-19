add_subdirectory(DFRendering)
add_subdirectory(NVPRendering)
add_subdirectory(PresentationScene)

if (OPTION_BUILD_GLAT_PREPROCESSOR)
	add_subdirectory(DFPreprocessing)
endif()