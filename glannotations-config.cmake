# GLANNOTATIONS_FOUND

# GLANNOTATIONS_LIBRARIES
# GLANNOTATIONS_INCLUDES

# GLANNOTATIONS_LIBRARY
# GLANNOTATIONS_LIBRARY_RELEASE
# GLANNOTATIONS_LIBRARY_DEBUG
# GLANNOTATIONS_INCLUDE_DIR

# GLANNOTATIONS_PREPROCESSOR_LIBRARY
# GLANNOTATIONS_PREPROCESSOR_LIBRARY_RELEASE
# GLANNOTATIONS_PREPROCESSOR_LIBRARY_DEBUG
# GLANNOTATIONS_PREPROCESSOR_INCLUDE_DIR

# GLANNOTATIONS_WRAPPER_LIBRARY
# GLANNOTATIONS_WRAPPER_LIBRARY_RELEASE
# GLANNOTATIONS_WRAPPER_LIBRARY_DEBUG
# GLANNOTATIONS_WRAPPER_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLANNOTATIONS_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLANNOTATIONS_DIR}" ENVGLANNOTATIONS_DIR)

set(LIB_PATHS   
    ${GLANNOTATIONS_DIR}/build
    ${GLANNOTATIONS_DIR}/build/Release
    ${GLANNOTATIONS_DIR}/build/Debug
    ${GLANNOTATIONS_DIR}/build-release
    ${GLANNOTATIONS_DIR}/build-debug
    ${ENVGLANNOTATIONS_DIR}/lib
    ${GLANNOTATIONS_DIR}/lib
    ${ENVPROGRAMFILES}/glannotations/lib
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

    if (${LIB_NAME} STREQUAL "glannotations")
        set(LIB_NAME_UPPER GLANNOTATIONS)
        set(LIBNAME glannotations)
    else()
        string(TOUPPER GLANNOTATIONS_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME glannotations-${LIB_NAME})
    endif()

    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVGLANNOTATIONS_DIR}/include
        ${ENVGLANNOTATIONS_DIR}/source/${LIBNAME}/include
        ${GLANNOTATIONS_DIR}/include
        ${GLANNOTATIONS_DIR}/source/${LIBNAME}/include
        ${ENVPROGRAMFILES}/glannotations/include
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

    set(GLANNOTATIONS_INCLUDES  ${GLANNOTATIONS_INCLUDES}  ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    set(GLANNOTATIONS_LIBRARIES ${GLANNOTATIONS_LIBRARIES} ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG
    message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro()

find(glannotations  glannotations/glannotations_api.h             ${LIB_PATHS})
find(preprocessor   glannotations-preprocessor/glannotations-preprocessor_api.h   ${LIB_PATHS})
find(wrapper        glannotations/wrapper/glannotations-wrapper_api.h ${LIB_PATHS})

# DEBUG
message("GLANNOTATIONS_INCLUDES  = ${GLANNOTATIONS_INCLUDES}")
message("GLANNOTATIONS_LIBRARIES = ${GLANNOTATIONS_LIBRARIES}")

find_package_handle_standard_args(GLANNOTATIONS DEFAULT_MSG GLANNOTATIONS_LIBRARIES GLANNOTATIONS_INCLUDES)
mark_as_advanced(GLANNOTATIONS_FOUND)
