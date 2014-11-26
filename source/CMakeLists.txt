# Generate meta-header
configure_file(glannotations-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/glannotations-version.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(glannotations)
if (OPTION_BUILD_GLANNOTATIONS_PREPROCESSOR)
	add_subdirectory(glannotations-preprocessor)
endif()
if (OPTION_BUILD_GLANNOTATIONS_WRAPPER)
	add_subdirectory(glannotations-wrapper)
endif()


# Examples
set(IDE_FOLDER "Examples")
add_subdirectory(examples)

# Tests
#set(IDE_FOLDER "Tests")
#add_subdirectory(tests)
