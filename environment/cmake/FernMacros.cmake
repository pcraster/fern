INCLUDE(CheckCXXSourceRuns)


MACRO(ADD_PARSER_GENERATION_COMMAND
        BASENAME)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${BASENAME}.map.in
        ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.map
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}-pskel.hxx
            ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}-pskel.cxx
        COMMAND
            ${XSD_EXECUTABLE} cxx-parser --xml-parser expat
                --type-map ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.map
                ${ARGN}
                ${CMAKE_CURRENT_SOURCE_DIR}/${BASENAME}.xsd
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.map
            ${CMAKE_CURRENT_SOURCE_DIR}/${BASENAME}.xsd
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
ENDMACRO()


# Targets can be added conditionally. When Fern components are selected during
# configuration, certain variables are set that tell us whether or not to
# include a target or group of targets. See FernConfiguration.cmake.
# TARGET_NAME   : Name of target, snake_case.
# DIRECTORY_NAME: Name of subdirectory containing the target.
MACRO(ADD_TARGET_CONDITIONALLY
        TARGET_NAME
        DIRECTORY_NAME)
    # Determine name of variable to check.
    STRING(TOUPPER ${TARGET_NAME} VARIABLE_NAME)
    SET(VARIABLE_NAME "FERN_FERN_${VARIABLE_NAME}_REQUIRED")

    # Make sure it is defined.
    IF(NOT DEFINED ${VARIABLE_NAME})
        MESSAGE(SEND_ERROR "Variable ${VARIABLE_NAME} is not defined")
    ENDIF()

    # Evaluate the variable and include target if result is true.
    IF(${VARIABLE_NAME})
        ADD_SUBDIRECTORY(${DIRECTORY_NAME})
    ENDIF()
ENDMACRO()


# Create a static library and an object library.
# BASENAME: Name of static library to create. The object library will be 
#           named ${BASENAME}_objects.
# SOURCES : Sources that are part of the libraries. Any argument that comes
#           after the BASENAME is treated as a source.
MACRO(ADD_LIBRARY_AND_OBJECT_LIBRARY
        BASENAME)
    SET(SOURCES ${ARGN})
    ADD_LIBRARY(${BASENAME}
        ${SOURCES}
    )
    ADD_LIBRARY(${BASENAME}_objects OBJECT
        ${SOURCES}
    )
ENDMACRO()


# Verify that headers are self-sufficient: they include the headers they need.
# OFFSET_PATHNAME : Pathname to root of headers.
# INCLUDES        : Pathnames where included headers can be found.
# LIBRARIES       : Libraries to link.
# FLAGS           : Compiler flags.
# HEADER_PATHNAMES: Pathnames to headers to verify.
# 
# Cache veriables will be set that are named after the headers:
# <header_name>_IS_STANDALONE
MACRO(VERIFY_HEADERS_ARE_SELF_SUFFICIENT)
    SET(OPTIONS "")
    SET(ONE_VALUE_ARGUMENTS OFFSET_PATHNAME FLAGS)
    SET(MULTI_VALUE_ARGUMENTS INCLUDES LIBRARIES HEADER_PATHNAMES)
    CMAKE_PARSE_ARGUMENTS(VERIFY_HEADERS "${OPTIONS}" "${ONE_VALUE_ARGUMENTS}"
        "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

    IF(VERIFY_HEADERS_UNPARSED_ARGUMENTS)
        MESSAGE(FATAL_ERROR
            "Macro called with unrecognized arguments: "
            "${VERIFY_HEADERS_UNPARSED_ARGUMENTS}"
        )
    ENDIF()

    SET(CMAKE_REQUIRED_FLAGS "${VERIFY_HEADERS_FLAGS}")
    # SET(CMAKE_REQUIRED_DEFINITIONS xxx)
    SET(CMAKE_REQUIRED_INCLUDES ${VERIFY_HEADERS_INCLUDES})
    SET(CMAKE_REQUIRED_LIBRARIES ${VERIFY_HEADERS_LIBRARIES})

    FOREACH(HEADER_PATHNAME ${VERIFY_HEADERS_HEADER_PATHNAMES})
        STRING(REPLACE ${VERIFY_HEADERS_OFFSET_PATHNAME} "" HEADER_NAME
            ${HEADER_PATHNAME})

        # Create variable name that contains the name of the header being
        # checked and is a valid macro name. It is passed to the compiler:
        # -D${VARIABLE_NAME}. That meanѕ that some characters cannot be in
        # the name.
        SET(VARIABLE_NAME ${HEADER_NAME})
        STRING(REPLACE /  _ VARIABLE_NAME ${VARIABLE_NAME})
        STRING(REPLACE \\ _ VARIABLE_NAME ${VARIABLE_NAME})
        STRING(REPLACE .  _ VARIABLE_NAME ${VARIABLE_NAME})

        # - Include the header twice to see whether the '#pragma once' is in
        #   place.
        # - Compile a dummy main to see whether the header includes everything
        #   it uses.
        CHECK_CXX_SOURCE_COMPILES("
            #include \"${HEADER_NAME}\"
            #include \"${HEADER_NAME}\"
            int main(int /* argc */, char** /* argv */) {
              return 0;
            }"
            ${VARIABLE_NAME})

        IF(NOT ${VARIABLE_NAME})
            MESSAGE(FATAL_ERROR
                "Header ${HEADER_NAME} is not self-sufficient. "
                "Inspect CMakeFiles/{CMakeError.log,CMakeOutput.log}."
            )
        ENDIF()
    ENDFOREACH()
ENDMACRO()
