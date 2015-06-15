# GLANNOTATIONS_FOUND

# GLANNOTATIONS_LIBRARIES
# GLANNOTATIONS_INCLUDE_DIRS
# GLANNOTATIONS_BINARIES

# GLANNOTATIONS_LIBRARY
# GLANNOTATIONS_LIBRARY_[RELEASE|DEBUG]
# GLANNOTATIONS_INCLUDE_DIR
# GLANNOTATIONS_BINARIES
# GLANNOTATIONS_BINARY_[RELEASE|DEBUG]

# GLANNOTATIONS_PREPROCESSOR_LIBRARY
# GLANNOTATIONS_PREPROCESSOR_LIBRARY_[RELEASE|DEBUG]
# GLANNOTATIONS_PREPROCESSOR_INCLUDE_DIR
# GLANNOTATIONS_BINARIES
# GLANNOTATIONS_BINARY_[RELEASE|DEBUG]

# GLANNOTATIONS_WRAPPER_LIBRARY
# GLANNOTATIONS_WRAPPER_LIBRARY_[RELEASE|DEBUG]
# GLANNOTATIONS_WRAPPER_INCLUDE_DIR
# GLANNOTATIONS_BINARIES
# GLANNOTATIONS_BINARY_[RELEASE|DEBUG]


include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLANNOTATIONS_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLANNOTATIONS_DIR}" ENVGLANNOTATIONS_DIR)

set(GLANNOTATIONS_INCLUDE_DIRS "")
set(GLANNOTATIONS_LIBRARIES "")
set(GLANNOTATIONS_BINARIES "")

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

    string(TOUPPER ${LIB_NAME} LIB_NAME_UPPER)
    string(REPLACE "-" "_" LIB_NAME_UPPER ${LIB_NAME_UPPER})
    
    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVGLANNOTATIONS_DIR}/include
        ${ENVGLANNOTATIONS_DIR}/source/${LIB_NAME}/include
        ${GLANNOTATIONS_DIR}/include
        ${GLANNOTATIONS_DIR}/source/${LIB_NAME}/include
        ${ENVPROGRAMFILES}/glannotations/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${header} resides")

    find_library(${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIB_NAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library")
    find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIB_NAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library")
    
    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    # DEBUG
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

    if (${LIB_NAME_UPPER}_LIBRARY)
        list(APPEND GLANNOTATIONS_INCLUDE_DIRS ${${LIB_NAME_UPPER}_INCLUDE_DIR})
        list(APPEND GLANNOTATIONS_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
    endif ()

    # find binaries
    if (WIN32 AND ${LIB_NAME_UPPER}_LIBRARIES)
        set(${LIB_NAME_UPPER}_BINARIES "")

        find_file(${LIB_NAME_UPPER}_BINARY_RELEASE
            NAMES ${LIB_NAME}.dll
            PATHS
            ${GLANNOTATIONS_DIR}/bin
            ${GLANNOTATIONS_DIR}/build/Release
            ${GLANNOTATIONS_DIR}/build-release
            DOC "The ${LIB_NAME_UPPER} binary")

        find_file(${LIB_NAME_UPPER}_BINARY_DEBUG
            NAMES ${LIB_NAME}d.dll
            PATHS
            ${GLANNOTATIONS_DIR}/bin
            ${GLANNOTATIONS_DIR}/build/Debug
            ${GLANNOTATIONS_DIR}/build-debug
            DOC "The ${LIB_NAME_UPPER} debug binary")

        if(NOT ${LIB_NAME_UPPER}_BINARY_RELEASE STREQUAL "${LIB_NAME_UPPER}_BINARY_RELEASE-NOTFOUND")
            list(APPEND ${LIB_NAME_UPPER}_BINARIES ${${LIB_NAME_UPPER}_BINARY_RELEASE})
        endif()

        if(NOT ${LIB_NAME_UPPER}_BINARY_DEBUG STREQUAL "${LIB_NAME_UPPER}_BINARY_DEBUG-NOTFOUND")
            list(APPEND ${LIB_NAME_UPPER}_BINARIES ${${LIB_NAME_UPPER}_BINARY_DEBUG})
        endif()

        if (${LIB_NAME_UPPER}_BINARIES)
            list(APPEND GLANNOTATIONS_BINARIES ${${LIB_NAME_UPPER}_BINARIES})
        endif()
    endif()
endmacro()

find(glannotations                glannotations/glannotations_api.h                           ${LIB_PATHS})
find(glannotations-preprocessor   glannotations-preprocessor/glannotations-preprocessor_api.h ${LIB_PATHS})
find(glannotations-wrapper        glannotations-wrapper/glannotations-wrapper_api.h           ${LIB_PATHS})

# DEBUG
# message("GLANNOTATIONS_INCLUDE_DIRS  = ${GLANNOTATIONS_INCLUDE_DIRS}")
# message("GLANNOTATIONS_LIBRARIES = ${GLANNOTATIONS_LIBRARIES}")

find_package_handle_standard_args(GLANNOTATIONS DEFAULT_MSG GLANNOTATIONS_LIBRARIES GLANNOTATIONS_INCLUDE_DIRS)
mark_as_advanced(GLANNOTATIONS_FOUND)
