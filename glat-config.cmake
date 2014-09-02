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

# GLAT_WRAPPER_LIBRARY
# GLAT_WRAPPER_LIBRARY_RELEASE
# GLAT_WRAPPER_LIBRARY_DEBUG
# GLAT_WRAPPER_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLAT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLAT_DIR}" ENVGLAT_DIR)

set(LIB_PATHS   
    ${GLAT_DIR}/build
    ${GLAT_DIR}/build/Release
    ${GLAT_DIR}/build/Debug
    ${GLAT_DIR}/build-release
    ${GLAT_DIR}/build-debug
    ${ENVGLAT_DIR}/lib
    ${GLAT_DIR}/lib
    ${ENVPROGRAMFILES}/glat/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)

macro (find LIB_NAME HEADER)
    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "glat")
        set(LIB_NAME_UPPER GLAT)
        set(LIBNAME glat)
    else()
        string(TOUPPER GLAT_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME glat-${LIB_NAME})
    endif()

    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVGLAT_DIR}/include
        ${ENVGLAT_DIR}/source/${LIBNAME}/include
        ${GLAT_DIR}/include
        ${GLAT_DIR}/source/${LIBNAME}/include
        ${ENVPROGRAMFILES}/glat/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${header} resides")

    find_library(${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library")
    find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library")
    
    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    set(GLAT_INCLUDES  ${GLAT_INCLUDES}  ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    set(GLAT_LIBRARIES ${GLAT_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG
    message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()

find(glat   glat/glat_api.h             ${LIB_PATHS})
find(preprocessor  glat-preprocessor/glat-preprocessor_api.h   ${LIB_PATHS})
find(wrapper glat/wrapper/glat-wrapper_api.h ${LIB_PATHS})

# DEBUG
message("GLAT_INCLUDES  = ${GLAT_INCLUDES}")
message("GLAT_LIBRARIES = ${GLAT_LIBRARIES}")

find_package_handle_standard_args(GLAT DEFAULT_MSG GLAT_LIBRARIES GLAT_INCLUDES)
mark_as_advanced(GLAT_FOUND)
