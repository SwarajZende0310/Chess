add_library(
  ${CHESS_CORE_TARGET_NAME} STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Application.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Application.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Core.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Core.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/AssetManager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/AssetManager.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Stage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Stage.cpp
 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Board.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Board.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Piece.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Piece.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/ChessState.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/ChessState.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Object.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Object.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/framework/Delegate.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/framework/Delegate.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/King.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/King.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/Queen.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/Queen.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/Rook.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/Rook.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/Knight.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/Knight.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/Bishop.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/Bishop.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/Pieces/Pawn.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Pieces/Pawn.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets/Widget.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/Widget.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets/Button.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/Button.cpp
)

target_include_directories(${CHESS_CORE_TARGET_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
set(SFML_LIB_NAME SFML)

FetchContent_Declare(
  ${SFML_LIB_NAME}
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.0
  GIT_SHALLOW FALSE)

FetchContent_MakeAvailable(${SFML_LIB_NAME})

target_link_libraries(${CHESS_CORE_TARGET_NAME} PUBLIC sfml-graphics)
target_link_libraries(${CHESS_CORE_TARGET_NAME} PUBLIC sfml-window)
target_link_libraries(${CHESS_CORE_TARGET_NAME} PUBLIC sfml-system)
target_link_libraries(${CHESS_CORE_TARGET_NAME} PUBLIC sfml-audio)

function(CopyLibToTarget LIB_NAME TARGET_NAME)
  add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIB_NAME}>
            $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()

copylibtotarget(sfml-graphics ${CHESS_CORE_TARGET_NAME})
copylibtotarget(sfml-window ${CHESS_CORE_TARGET_NAME})
copylibtotarget(sfml-system ${CHESS_CORE_TARGET_NAME})
copylibtotarget(sfml-audio ${CHESS_CORE_TARGET_NAME})
