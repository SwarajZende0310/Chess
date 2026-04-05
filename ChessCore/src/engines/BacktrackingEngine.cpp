#include "engines/BacktrackingEngine.h"

#include <algorithm>
#include <cctype>
#include <chrono>

namespace chess
{
    namespace
    {
        constexpr float CHECKMATE_SCORE = 100000.0f;

        struct ParsedFen
        {
            List<List<char>> board{8, List<char>(8, '\0')};
            bool whiteToMove = true;
            std::string castlingRights = "-";
            std::string enPassant = "-";
            int halfmoveClock = 0;
            int fullmoveNumber = 1;
        };

        bool IsWhitePiece(char piece)
        {
            return std::isupper(static_cast<unsigned char>(piece)) != 0;
        }

        bool IsBlackPiece(char piece)
        {
            return std::islower(static_cast<unsigned char>(piece)) != 0;
        }

        bool IsPieceOfSide(char piece, bool whiteMove)
        {
            return whiteMove ? IsWhitePiece(piece) : IsBlackPiece(piece);
        }

        bool IsEnemyPiece(char piece, bool whiteMove)
        {
            return piece != '\0' && !IsPieceOfSide(piece, whiteMove);
        }

        std::pair<int, int> SquareToBoardIndex(const std::string& square)
        {
            if(square.size() != 2 || square[0] < 'a' || square[0] > 'h' || square[1] < '1' || square[1] > '8')
            {
                return {-1, -1};
            }

            return {8 - (square[1] - '0'), square[0] - 'a'};
        }

        std::string BoardIndexToSquare(int row, int col)
        {
            return std::string{static_cast<char>('a' + col), static_cast<char>('8' - row)};
        }

        ParsedFen ParseFenString(const std::string& fen)
        {
            ParsedFen parsedFen;

            std::stringstream fenStream(fen);
            std::string piecePlacement;
            std::string activeColor;

            fenStream >> piecePlacement >> activeColor >> parsedFen.castlingRights
                      >> parsedFen.enPassant >> parsedFen.halfmoveClock >> parsedFen.fullmoveNumber;

            parsedFen.whiteToMove = activeColor != "b";
            if(parsedFen.castlingRights.empty())
            {
                parsedFen.castlingRights = "-";
            }
            if(parsedFen.enPassant.empty())
            {
                parsedFen.enPassant = "-";
            }

            std::stringstream rows(piecePlacement);
            for(int row = 0; row < 8; ++row)
            {
                std::string rank;
                std::getline(rows, rank, '/');

                int col = 0;
                for(char symbol : rank)
                {
                    if(symbol >= '1' && symbol <= '8')
                    {
                        col += symbol - '0';
                        continue;
                    }

                    if(col < 8)
                    {
                        parsedFen.board[row][col] = symbol;
                        ++col;
                    }
                }
            }

            return parsedFen;
        }

        std::string BuildFenString(const ParsedFen& parsedFen)
        {
            std::string fen;
            for(int row = 0; row < 8; ++row)
            {
                int emptyCount = 0;
                for(int col = 0; col < 8; ++col)
                {
                    const char piece = parsedFen.board[row][col];
                    if(piece == '\0')
                    {
                        ++emptyCount;
                        continue;
                    }

                    if(emptyCount > 0)
                    {
                        fen += static_cast<char>('0' + emptyCount);
                        emptyCount = 0;
                    }

                    fen += piece;
                }

                if(emptyCount > 0)
                {
                    fen += static_cast<char>('0' + emptyCount);
                }

                if(row != 7)
                {
                    fen += '/';
                }
            }

            fen += parsedFen.whiteToMove ? " w " : " b ";
            fen += parsedFen.castlingRights.empty() ? "-" : parsedFen.castlingRights;
            fen += " ";
            fen += parsedFen.enPassant.empty() ? "-" : parsedFen.enPassant;
            fen += " ";
            fen += std::to_string(parsedFen.halfmoveClock);
            fen += " ";
            fen += std::to_string(parsedFen.fullmoveNumber);

            return fen;
        }

        void RemoveCastlingRight(std::string& castlingRights, char castlingFlag)
        {
            castlingRights.erase(std::remove(castlingRights.begin(), castlingRights.end(), castlingFlag), castlingRights.end());
            if(castlingRights.empty())
            {
                castlingRights = "-";
            }
        }

        void UpdateCastlingRightsForMove(ParsedFen& parsedFen, char movedPiece, int fromRow, int fromCol, int toRow, int toCol, char capturedPiece)
        {
            if(movedPiece == 'K')
            {
                RemoveCastlingRight(parsedFen.castlingRights, 'K');
                RemoveCastlingRight(parsedFen.castlingRights, 'Q');
            }
            else if(movedPiece == 'k')
            {
                RemoveCastlingRight(parsedFen.castlingRights, 'k');
                RemoveCastlingRight(parsedFen.castlingRights, 'q');
            }
            else if(movedPiece == 'R')
            {
                if(fromRow == 7 && fromCol == 0)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'Q');
                }
                else if(fromRow == 7 && fromCol == 7)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'K');
                }
            }
            else if(movedPiece == 'r')
            {
                if(fromRow == 0 && fromCol == 0)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'q');
                }
                else if(fromRow == 0 && fromCol == 7)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'k');
                }
            }

            if(capturedPiece == 'R')
            {
                if(toRow == 7 && toCol == 0)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'Q');
                }
                else if(toRow == 7 && toCol == 7)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'K');
                }
            }
            else if(capturedPiece == 'r')
            {
                if(toRow == 0 && toCol == 0)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'q');
                }
                else if(toRow == 0 && toCol == 7)
                {
                    RemoveCastlingRight(parsedFen.castlingRights, 'k');
                }
            }
        }

        bool ApplyMoveToPosition(ParsedFen& parsedFen, const std::string& move)
        {
            if(move.size() < 4)
            {
                return false;
            }

            const std::pair<int, int> from = SquareToBoardIndex(move.substr(0, 2));
            const std::pair<int, int> to = SquareToBoardIndex(move.substr(2, 2));
            if(from.first < 0 || to.first < 0)
            {
                return false;
            }

            char piece = parsedFen.board[from.first][from.second];
            if(piece == '\0' || !IsPieceOfSide(piece, parsedFen.whiteToMove))
            {
                return false;
            }

            char capturedPiece = parsedFen.board[to.first][to.second];
            const bool isPawnMove = std::tolower(static_cast<unsigned char>(piece)) == 'p';
            const bool isDiagonalPawnMove = isPawnMove && from.second != to.second;
            const bool isEnPassantCapture = isDiagonalPawnMove && capturedPiece == '\0' && parsedFen.enPassant != "-" &&
                                            BoardIndexToSquare(to.first, to.second) == parsedFen.enPassant;

            UpdateCastlingRightsForMove(parsedFen, piece, from.first, from.second, to.first, to.second, capturedPiece);

            parsedFen.board[from.first][from.second] = '\0';

            if(isEnPassantCapture)
            {
                const int capturedPawnRow = parsedFen.whiteToMove ? to.first + 1 : to.first - 1;
                if(capturedPawnRow < 0 || capturedPawnRow >= 8)
                {
                    return false;
                }
                capturedPiece = parsedFen.board[capturedPawnRow][to.second];
                parsedFen.board[capturedPawnRow][to.second] = '\0';
            }

            const bool isCastlingMove = std::tolower(static_cast<unsigned char>(piece)) == 'k' &&
                                        std::abs(to.second - from.second) == 2;
            if(isCastlingMove)
            {
                if(to.second == 6)
                {
                    parsedFen.board[to.first][5] = parsedFen.board[to.first][7];
                    parsedFen.board[to.first][7] = '\0';
                }
                else if(to.second == 2)
                {
                    parsedFen.board[to.first][3] = parsedFen.board[to.first][0];
                    parsedFen.board[to.first][0] = '\0';
                }
            }

            char placedPiece = piece;
            if(move.size() == 5)
            {
                const char promotionPiece = move[4];
                placedPiece = parsedFen.whiteToMove
                    ? static_cast<char>(std::toupper(static_cast<unsigned char>(promotionPiece)))
                    : static_cast<char>(std::tolower(static_cast<unsigned char>(promotionPiece)));
            }
            parsedFen.board[to.first][to.second] = placedPiece;

            parsedFen.enPassant = "-";
            if(isPawnMove && std::abs(to.first - from.first) == 2)
            {
                parsedFen.enPassant = BoardIndexToSquare((from.first + to.first) / 2, from.second);
            }

            parsedFen.halfmoveClock = (isPawnMove || capturedPiece != '\0') ? 0 : parsedFen.halfmoveClock + 1;
            if(!parsedFen.whiteToMove)
            {
                ++parsedFen.fullmoveNumber;
            }

            parsedFen.whiteToMove = !parsedFen.whiteToMove;
            return true;
        }

    }

    BacktrackingEngine::BacktrackingEngine(int maxDepth, size_t nodeBudget, int timeBudgetMs)
        :mMaxDepth{maxDepth},
         mBestMove{},
         mMoveHistory{},
         mVisitedNodes{0},
         mNodeBudget{nodeBudget},
         mSearchAborted{false},
         mSearchStartTime{},
         mTimeBudgetMs{timeBudgetMs},
         mCancelFlag{nullptr},
         mDefaultNodeBudget{nodeBudget},
         mDefaultTimeBudgetMs{timeBudgetMs}
    {
    }

    float BacktrackingEngine::GetEvaluation(std::string &fen, int depth)
    {
        EngineSearchRequest request;
        request.fen = fen;
        request.maxDepth = depth < 0 ? mMaxDepth : depth;

        const EngineSearchResult result = AnalyzePosition(request);
        mBestMove = result.bestMove;
        return result.evaluation;
    }

    EngineSearchResult BacktrackingEngine::AnalyzePosition(const EngineSearchRequest& request)
    {
        EngineSearchResult bestResult;
        bestResult.evaluation = static_cast<float>(GetEvaluationOnPieceValue(request.fen));

        const int targetDepth = request.maxDepth < 1 ? mMaxDepth : request.maxDepth;
        mNodeBudget = request.nodeBudget == 0 ? mDefaultNodeBudget : request.nodeBudget;
        mTimeBudgetMs = request.timeBudgetMs <= 0 ? mDefaultTimeBudgetMs : request.timeBudgetMs;
        mCancelFlag = request.cancelFlag;
        mVisitedNodes = 0;
        mSearchStartTime = std::chrono::steady_clock::now();
        mBestMove.clear();

        for(int currentDepth = 1; currentDepth <= targetDepth; ++currentDepth)
        {
            std::string workingFen = request.fen;
            mMoveHistory.clear();
            mBestMove.clear();
            mSearchAborted = false;

            const float evaluation = Search(workingFen, currentDepth, -CHECKMATE_SCORE, CHECKMATE_SCORE, currentDepth);
            if(mSearchAborted)
            {
                break;
            }

            bestResult.evaluation = evaluation;
            bestResult.bestMove = mBestMove;
            bestResult.completedDepth = currentDepth;
        }

        bestResult.visitedNodes = mVisitedNodes;
        bestResult.elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - mSearchStartTime).count();
        bestResult.aborted = (bestResult.completedDepth < targetDepth);
        mBestMove = bestResult.bestMove;
        return bestResult;
    }

    float BacktrackingEngine::Search(std::string& fen, int depth, float alpha, float beta, int rootDepth)
    {
        if(ShouldAbortSearch())
        {
            mSearchAborted = true;
            return static_cast<float>(GetEvaluationOnPieceValue(fen));
        }

        ++mVisitedNodes;
        if(ShouldAbortSearch())
        {
            mSearchAborted = true;
            return static_cast<float>(GetEvaluationOnPieceValue(fen));
        }

        std::stringstream board(fen);
        std::string piecePlacement;
        std::string whoseMove;

        std::getline(board, piecePlacement, ' ');
        std::getline(board, whoseMove, ' ');

        const bool whiteMove = (whoseMove == "w");
        List<std::string> possibleMoves = GetAllPossibleMoves(fen, whiteMove);

        if(possibleMoves.empty())
        {
            ParsedFen parsedFen = ParseFenString(fen);
            const std::set<std::pair<int, int>> attackedSquares = GetAttackedSquares(parsedFen.board, whiteMove);
            if(IsKingInCheck(parsedFen.board, whiteMove, attackedSquares))
            {
                return whiteMove ? -CHECKMATE_SCORE + depth : CHECKMATE_SCORE - depth;
            }
            return 0.0f;
        }

        if(depth == 0)
        {
            return static_cast<float>(GetEvaluationOnPieceValue(fen));
        }

        float evaluation = whiteMove ? -CHECKMATE_SCORE : CHECKMATE_SCORE;
        for(const std::string& move : possibleMoves)
        {
            if(!MakeMove(fen, move))
            {
                continue;
            }

            const float evaluationAfterMove = Search(fen, depth - 1, alpha, beta, rootDepth);
            UndoMove(fen, move);
            if(mSearchAborted)
            {
                break;
            }

            if(whiteMove)
            {
                if(evaluationAfterMove > evaluation)
                {
                    evaluation = evaluationAfterMove;
                    if(depth == rootDepth)
                    {
                        mBestMove = move;
                    }
                }
                alpha = std::max(alpha, evaluation);
            }
            else
            {
                if(evaluationAfterMove < evaluation)
                {
                    evaluation = evaluationAfterMove;
                    if(depth == rootDepth)
                    {
                        mBestMove = move;
                    }
                }
                beta = std::min(beta, evaluation);
            }

            if(beta <= alpha)
            {
                break;
            }
        }

        return evaluation;
    }

    bool BacktrackingEngine::ShouldAbortSearch() const
    {
        if(mSearchAborted)
        {
            return true;
        }

        if(mCancelFlag != nullptr && mCancelFlag->load())
        {
            return true;
        }

        if(mNodeBudget > 0 && mVisitedNodes >= mNodeBudget)
        {
            return true;
        }

        if(mTimeBudgetMs > 0)
        {
            const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - mSearchStartTime).count();
            if(elapsedMs >= mTimeBudgetMs)
            {
                return true;
            }
        }

        return false;
    }

    std::string BacktrackingEngine::GetBestMove() const
    {
        return mBestMove;
    }

    std::string BacktrackingEngine::GetEngineVersion() const
    {
        return "1.0";
    }
    
    std::string BacktrackingEngine::GetEngineName() const
    {
        return "Backtracking";
    }

    int BacktrackingEngine::GetEvaluationOnPieceValue(const std::string &fen)
    {
        std::unordered_map<char,int> pieces = {{'P',1}, {'B',3}, {'N',3}, {'R',5}, {'Q',9},
                                               {'p',-1}, {'b',-3}, {'n',-3}, {'r',-5}, {'q',-9}};

        std::stringstream fenFormat(fen);
        std::string piecePlacement;
        std::getline(fenFormat, piecePlacement, ' ');

        std::stringstream board(piecePlacement);
        int evaluation = 0;

        for(size_t rowIndex = 0; rowIndex < 8; ++rowIndex)
        {
            std::string row;
            std::getline(board, row, '/');
            for(char symbol : row)
            {
                if(pieces.count(symbol) > 0)
                {
                    evaluation += pieces[symbol];
                }
            }
        }

        return evaluation;
    }

    List<std::string> BacktrackingEngine::GetAllPossibleMoves(const std::string &fen, bool whiteMove)
    {
        ParsedFen parsedFen = ParseFenString(fen);
        List<std::string> candidateMoves;
        candidateMoves.reserve(64);

        const auto pushLegalMove = [&](const std::string& move)
        {
            ParsedFen nextPosition = parsedFen;
            if(!ApplyMoveToPosition(nextPosition, move))
            {
                return;
            }

            const std::set<std::pair<int, int>> attackedSquares = GetAttackedSquares(nextPosition.board, whiteMove);
            if(!IsKingInCheck(nextPosition.board, whiteMove, attackedSquares))
            {
                candidateMoves.emplace_back(move);
            }
        };

        const std::set<std::pair<int, int>> attackedSquares = GetAttackedSquares(parsedFen.board, whiteMove);

        for(int row = 0; row < 8; ++row)
        {
            for(int col = 0; col < 8; ++col)
            {
                const char piece = parsedFen.board[row][col];
                if(piece == '\0' || !IsPieceOfSide(piece, whiteMove))
                {
                    continue;
                }

                const std::string fromSquare = BoardIndexToSquare(row, col);
                switch (std::tolower(static_cast<unsigned char>(piece)))
                {
                case 'p':
                    {
                        const int forwardStep = whiteMove ? -1 : 1;
                        const int startRow = whiteMove ? 6 : 1;
                        const int promotionRow = whiteMove ? 0 : 7;
                        const int nextRow = row + forwardStep;

                        if(InBounds(nextRow, col) && parsedFen.board[nextRow][col] == '\0')
                        {
                            const std::string movePrefix = fromSquare + BoardIndexToSquare(nextRow, col);
                            if(nextRow == promotionRow)
                            {
                                pushLegalMove(movePrefix + "q");
                                pushLegalMove(movePrefix + "r");
                                pushLegalMove(movePrefix + "b");
                                pushLegalMove(movePrefix + "n");
                            }
                            else
                            {
                                pushLegalMove(movePrefix);
                            }

                            const int doubleStepRow = row + (2 * forwardStep);
                            if(row == startRow && InBounds(doubleStepRow, col) && parsedFen.board[doubleStepRow][col] == '\0')
                            {
                                pushLegalMove(fromSquare + BoardIndexToSquare(doubleStepRow, col));
                            }
                        }

                        for(int captureOffset = -1; captureOffset <= 1; captureOffset += 2)
                        {
                            const int captureCol = col + captureOffset;
                            const int captureRow = row + forwardStep;
                            if(!InBounds(captureRow, captureCol))
                            {
                                continue;
                            }

                            const char targetPiece = parsedFen.board[captureRow][captureCol];
                            const std::string targetSquare = BoardIndexToSquare(captureRow, captureCol);
                            const bool isEnPassant = parsedFen.enPassant != "-" && parsedFen.enPassant == targetSquare;

                            if(!IsEnemyPiece(targetPiece, whiteMove) && !isEnPassant)
                            {
                                continue;
                            }

                            const std::string movePrefix = fromSquare + targetSquare;
                            if(captureRow == promotionRow)
                            {
                                pushLegalMove(movePrefix + "q");
                                pushLegalMove(movePrefix + "r");
                                pushLegalMove(movePrefix + "b");
                                pushLegalMove(movePrefix + "n");
                            }
                            else
                            {
                                pushLegalMove(movePrefix);
                            }
                        }
                        break;
                    }
                case 'n':
                    {
                        static const int offsetRow[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
                        static const int offsetCol[8] = {-2, -1, 1, 2, 2, 1, -1, -2};

                        for(int index = 0; index < 8; ++index)
                        {
                            const int nextRow = row + offsetRow[index];
                            const int nextCol = col + offsetCol[index];
                            if(!InBounds(nextRow, nextCol) || IsPieceOfSide(parsedFen.board[nextRow][nextCol], whiteMove))
                            {
                                continue;
                            }

                            pushLegalMove(fromSquare + BoardIndexToSquare(nextRow, nextCol));
                        }
                        break;
                    }
                case 'b':
                case 'r':
                case 'q':
                    {
                        static const int bishopOffsetRow[4] = {1, 1, -1, -1};
                        static const int bishopOffsetCol[4] = {1, -1, 1, -1};
                        static const int rookOffsetRow[4] = {1, -1, 0, 0};
                        static const int rookOffsetCol[4] = {0, 0, 1, -1};

                        const int* offsetRow = nullptr;
                        const int* offsetCol = nullptr;
                        int directionCount = 0;

                        if(std::tolower(static_cast<unsigned char>(piece)) == 'b')
                        {
                            offsetRow = bishopOffsetRow;
                            offsetCol = bishopOffsetCol;
                            directionCount = 4;
                        }
                        else if(std::tolower(static_cast<unsigned char>(piece)) == 'r')
                        {
                            offsetRow = rookOffsetRow;
                            offsetCol = rookOffsetCol;
                            directionCount = 4;
                        }
                        else
                        {
                            static const int queenOffsetRow[8] = {1, 1, -1, -1, 1, -1, 0, 0};
                            static const int queenOffsetCol[8] = {1, -1, 1, -1, 0, 0, 1, -1};
                            offsetRow = queenOffsetRow;
                            offsetCol = queenOffsetCol;
                            directionCount = 8;
                        }

                        for(int direction = 0; direction < directionCount; ++direction)
                        {
                            int nextRow = row + offsetRow[direction];
                            int nextCol = col + offsetCol[direction];
                            while(InBounds(nextRow, nextCol))
                            {
                                if(IsPieceOfSide(parsedFen.board[nextRow][nextCol], whiteMove))
                                {
                                    break;
                                }

                                pushLegalMove(fromSquare + BoardIndexToSquare(nextRow, nextCol));
                                if(parsedFen.board[nextRow][nextCol] != '\0')
                                {
                                    break;
                                }

                                nextRow += offsetRow[direction];
                                nextCol += offsetCol[direction];
                            }
                        }
                        break;
                    }
                case 'k':
                    {
                        static const int offsetRow[8] = {1, 1, -1, -1, 1, -1, 0, 0};
                        static const int offsetCol[8] = {1, -1, 1, -1, 0, 0, 1, -1};

                        for(int index = 0; index < 8; ++index)
                        {
                            const int nextRow = row + offsetRow[index];
                            const int nextCol = col + offsetCol[index];
                            if(!InBounds(nextRow, nextCol) || IsPieceOfSide(parsedFen.board[nextRow][nextCol], whiteMove))
                            {
                                continue;
                            }

                            if(attackedSquares.find({nextRow, nextCol}) == attackedSquares.end())
                            {
                                pushLegalMove(fromSquare + BoardIndexToSquare(nextRow, nextCol));
                            }
                        }

                        if(whiteMove && row == 7 && col == 4 && attackedSquares.find({7, 4}) == attackedSquares.end())
                        {
                            if(parsedFen.castlingRights.find('K') != std::string::npos &&
                               parsedFen.board[7][5] == '\0' && parsedFen.board[7][6] == '\0' &&
                               attackedSquares.find({7, 5}) == attackedSquares.end() &&
                               attackedSquares.find({7, 6}) == attackedSquares.end() &&
                               parsedFen.board[7][7] == 'R')
                            {
                                pushLegalMove("e1g1");
                            }

                            if(parsedFen.castlingRights.find('Q') != std::string::npos &&
                               parsedFen.board[7][1] == '\0' && parsedFen.board[7][2] == '\0' && parsedFen.board[7][3] == '\0' &&
                               attackedSquares.find({7, 3}) == attackedSquares.end() &&
                               attackedSquares.find({7, 2}) == attackedSquares.end() &&
                               parsedFen.board[7][0] == 'R')
                            {
                                pushLegalMove("e1c1");
                            }
                        }
                        else if(!whiteMove && row == 0 && col == 4 && attackedSquares.find({0, 4}) == attackedSquares.end())
                        {
                            if(parsedFen.castlingRights.find('k') != std::string::npos &&
                               parsedFen.board[0][5] == '\0' && parsedFen.board[0][6] == '\0' &&
                               attackedSquares.find({0, 5}) == attackedSquares.end() &&
                               attackedSquares.find({0, 6}) == attackedSquares.end() &&
                               parsedFen.board[0][7] == 'r')
                            {
                                pushLegalMove("e8g8");
                            }

                            if(parsedFen.castlingRights.find('q') != std::string::npos &&
                               parsedFen.board[0][1] == '\0' && parsedFen.board[0][2] == '\0' && parsedFen.board[0][3] == '\0' &&
                               attackedSquares.find({0, 3}) == attackedSquares.end() &&
                               attackedSquares.find({0, 2}) == attackedSquares.end() &&
                               parsedFen.board[0][0] == 'r')
                            {
                                pushLegalMove("e8c8");
                            }
                        }
                        break;
                    }
                default:
                    break;
                }
            }
        }

        return candidateMoves;
    }

    bool BacktrackingEngine::MakeMove(std::string &fen, std::string move)
    {
        ParsedFen parsedFen = ParseFenString(fen);
        if(!ApplyMoveToPosition(parsedFen, move))
        {
            return false;
        }

        mMoveHistory.push_back({fen, move});
        fen = BuildFenString(parsedFen);
        return true;
    }

    bool BacktrackingEngine::UndoMove(std::string &fen, std::string move)
    {
        if(mMoveHistory.empty())
        {
            return false;
        }

        const MoveHistoryEntry lastMove = mMoveHistory.back();
        mMoveHistory.pop_back();

        if(lastMove.move != move)
        {
            return false;
        }

        fen = lastMove.fenBeforeMove;
        return true;
    }

    std::set<std::pair<int, int>> BacktrackingEngine::GetAttackedSquares(const List<List<char>> &board, bool whiteMove)
    {
        std::set<std::pair<int,int>> attackedSquares;
        
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(board[i][j] == '\0') 
                {
                    continue;
                }
                
                if((whiteMove && std::isupper(static_cast<unsigned char>(board[i][j])) != 0) 
                   || (!whiteMove && std::islower(static_cast<unsigned char>(board[i][j])) != 0))
                {
                    continue;
                }

                switch (std::tolower(static_cast<unsigned char>(board[i][j])))
                {
                case 'p':
                    {
                        const int row = whiteMove ? i + 1 : i - 1;
                        if(InBounds(row, j - 1))
                        {
                            attackedSquares.insert({row, j - 1});
                        }
                        if(InBounds(row, j + 1))
                        {
                            attackedSquares.insert({row, j + 1});
                        }
                        break;
                    }
                case 'b':
                    {
                        int offsetRow[4] = { 1,  1, -1, -1};
                        int offsetCol[4] = { 1, -1,  1, -1};
                        
                        AddSquaresInRayAttacks(board, i, j, offsetRow, offsetCol, 4, attackedSquares); 
                        break;
                    }
                case 'n':
                    {
                        int offsetRow[8] = {-1, -2, -2, -1,  1,  2,  2,  1};
                        int offsetCol[8] = {-2, -1,  1,  2,  2,  1, -1, -2};
                        
                        for(int k = 0; k < 8; k++)
                        {
                            int row = i + offsetRow[k];
                            int col = j + offsetCol[k];

                            if(InBounds(row, col))
                            {
                                attackedSquares.insert({row, col});
                            }
                        }
                        break;
                    }
                case 'r':
                    {
                        int offsetRow[4] = { 1, -1,  0,  0};
                        int offsetCol[4] = { 0,  0,  1, -1};
                        
                        AddSquaresInRayAttacks(board, i, j, offsetRow, offsetCol, 4, attackedSquares);
                        break;
                    }
                case 'q':
                    {
                        int offsetRow[8] = { 1,  1, -1, -1,  1, -1,  0,  0};
                        int offsetCol[8] = { 1, -1,  1, -1,  0,  0,  1, -1};

                        AddSquaresInRayAttacks(board, i, j, offsetRow, offsetCol, 8, attackedSquares);
                        break;
                    }
                case 'k':
                    {
                        int offsetRow[8] = { 1,  1, -1, -1,  1, -1,  0,  0};
                        int offsetCol[8] = { 1, -1,  1, -1,  0,  0,  1, -1};

                        for(int k = 0; k < 8 ; k++)
                        {
                            int row = i + offsetRow[k];
                            int col = j + offsetCol[k];
                            if(InBounds(row, col))
                            {
                                attackedSquares.insert({row, col});
                            }
                        }
                        break;
                    }
                default:
                    break;
                }
            }
        }

        return attackedSquares;
    }

    bool chess::BacktrackingEngine::InBounds(int row, int col)
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    void BacktrackingEngine::AddSquaresInRayAttacks(const List<List<char>> &board, int startRow, int startCol, 
                                                    int *dirOffsetRow, int *dirOffsetCol, int offsetLen, 
                                                    std::set<std::pair<int, int>> &attackedSquares)
    {
        for(int i = 0; i < offsetLen; i++)
        {
            int row = startRow + dirOffsetRow[i];
            int col = startCol + dirOffsetCol[i];
            while(InBounds(row,col))
            {
                attackedSquares.insert({row,col});
                if(board[row][col] != '\0')
                {
                    break;
                }
                row += dirOffsetRow[i];
                col += dirOffsetCol[i];
            }
        }
    }

    bool BacktrackingEngine::IsKingInCheck(const List<List<char>> &board, bool whiteMove, const std::set<std::pair<int, int>> &attackedSquares)
    {
        std::pair<int,int> kingsPosition{-1, -1};
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(whiteMove && board[i][j] == 'K')
                {
                    kingsPosition = {i, j};
                    break;
                }
                if(!whiteMove && board[i][j] == 'k')
                {
                    kingsPosition = {i, j};
                    break;
                }
            }
        }

        return attackedSquares.find(kingsPosition) != attackedSquares.end();
    }
}
