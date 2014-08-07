# Generate meta-header
configure_file(glat-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/glat-version.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(glat)
if (OPTION_BUILD_GLAT_PREPROCESSOR)
	add_subdirectory(glat_preprocessor)
endif()
if (OPTION_BUILD_GLAT_WRAPPER)
	add_subdirectory(glat_wrapper)
endif()


# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
#set(IDE_FOLDER "Tests")
#add_subdirectory(tests)
