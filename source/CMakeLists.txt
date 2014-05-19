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

# Examples
if(OPTION_BUILD_EXAMPLES)
	set(IDE_FOLDER "Examples")
	add_subdirectory(examples)
endif()

# Tests
#set(IDE_FOLDER "Tests")
#add_subdirectory(tests)
