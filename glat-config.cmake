# GLAT_FOUND

# GLAT_LIBRARIES
# GLAT_INCLUDES

# GLAT_LIBRARY
# GLAT_LIBRARY_RELEASE
# GLAT_LIBRARY_DEBUG
# GLAT_INCLUDE_DIR

# GLAT_PREPROCESSOR_LIBRARY
# GLAT_PREPROCESSOR_LIBRARY_RELEASE
# GLAT_PREPROCESSOR_LIBRARY_DEBUG
# GLAT_PREPROCESSOR_INCLUDE_DIR

function (find LIB_NAME HEADER)

    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "glat")
        set(LIB_NAME_UPPER GLAT)
        set(LIBNAME glat)
    else()
        string(TOUPPER GLAT_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME ${LIB_NAME})
    endif()

    find_path(
	${LIB_NAME_UPPER}_INCLUDE_DIR
	${HEADER}
        ${ENV_GLAT_DIR}/include
        ${ENV_GLAT_DIR}/source/${LIB_NAME}/include
        ${GLAT_DIR}/include
        ${GLAT_DIR}/source/${LIB_NAME}/include
        ${ENV_PROGRAMFILES}/glat/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${HEADER} resides"
    )


    find_library(
	${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library"
    )
    find_library(
	${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library"
    )
    

    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()


    set(GLAT_INCLUDES  ${GLAT_INCLUDES}  ${${LIB_NAME_UPPER}_INCLUDE_DIR} PARENT_SCOPE)
    set(GLAT_LIBRARIES ${GLAT_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY} PARENT_SCOPE)

    # DEBUG MESSAGES
#    message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
#    message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
#    message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
#    message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endfunction(find)








# load standard CMake arguments (c.f. http://stackoverflow.com/questions/7005782/cmake-include-findpackagehandlestandardargs-cmake)
include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLAT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENV_PROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLAT_DIR}" ENV_GLAT_DIR)

set(LIB_PATHS   
    ${GLAT_DIR}/build
    ${GLAT_DIR}/build/Release
    ${GLAT_DIR}/build/Debug
    ${GLAT_DIR}/build-release
    ${GLAT_DIR}/build-debug
    ${GLAT_DIR}/lib
    ${ENV_GLAT_DIR}/lib
    ${ENV_PROGRAMFILES}/GLAT/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)


# Find libraries
find(glat glat/glat_api.h ${LIB_PATHS})
find(glat_preprocessor glat_preprocessor/preprocessor_api.h ${LIB_PATHS})


# DEBUG
#message("GLAT_INCLUDES  = ${GLAT_INCLUDES}")
#message("GLAT_LIBRARIES = ${GLAT_LIBRARIES}")

find_package_handle_standard_args(GLAT DEFAULT_MSG GLAT_LIBRARIES GLAT_INCLUDES)
mark_as_advanced(GLAT_FOUND)
