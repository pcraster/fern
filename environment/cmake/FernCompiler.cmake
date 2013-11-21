SET(LIBRARY_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all libraries."
)
SET(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single directory for all executables."
)
MARK_AS_ADVANCED(
    LIBRARY_OUTPUT_PATH
    EXECUTABLE_OUTPUT_PATH
)


INCLUDE(CheckCXXCompilerFlag)

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-qual -Wwrite-strings -Werror=strict-aliasing -std=c++11 -pedantic"
    )
ENDIF()

# Add the PIC compiler flag if needed.
IF(UNIX AND NOT WIN32)
    IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
        CHECK_CXX_COMPILER_FLAG("-fPIC" WITH_FPIC)
        IF(WITH_FPIC)
            ADD_DEFINITIONS(-fPIC)
        ENDIF()
    ENDIF()
ENDIF()
