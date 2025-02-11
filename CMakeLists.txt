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