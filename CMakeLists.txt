cmake_minimum_required(VERSION 3.31.3)

project(
    Chess
    VERSION 1.0.0
    LANGUAGES C CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CHESS_CORE_TARGET_NAME ChessCore)
set(CHESS_GAME_TARGET_NAME ChessGame)

add_subdirectory(ChessCore)
add_subdirectory(ChessGame)

# ============================================================
# DOXYGEN DOCUMENTATION SETUP
# ============================================================
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")

    # Input and output paths
    set(DOXYGEN_INPUT_DIR
        ${CMAKE_SOURCE_DIR}/ChessCore/include
        ${CMAKE_SOURCE_DIR}/ChessCore/src
        ${CMAKE_SOURCE_DIR}/ChessGame/include
        ${CMAKE_SOURCE_DIR}/ChessGame/src
    )

    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)

    # Doxygen template configuration file
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Pass variables to Doxygen configuration
    #configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # Convert CMake list to space-separated string for Doxygen
    string(REPLACE ";" " " DOXYGEN_INPUT_DIR "${DOXYGEN_INPUT_DIR}")
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)


    # Add custom target for generating docs
    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

    message(STATUS "Documentation target 'doc_doxygen' added.")
else()
    message(WARNING "Doxygen not found. Documentation target will not be available.")
endif()
