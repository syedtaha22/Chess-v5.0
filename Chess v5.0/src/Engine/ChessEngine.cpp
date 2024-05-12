#include "../../headers/Engine/ChessEngine.h"



ChessEngine::ChessEngine(int Color) {
    state.EngineColor = Color;
    state.initPieceValues();
}

void ChessEngine::shuffleMoves(std::vector<std::string>& possibleMoves) {
    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine(static_cast<unsigned int>(seed)));
}

std::string ChessEngine::GenerateMove(const ChessBoard& board) {
    std::unique_ptr<Timer> timer = std::make_unique<Timer>();
    int bestScore = -infinity;
    
    std::string bestMove;
    
    Heuristics.NumberofMovesLookedAhead = 0;
    Heuristics.TranspositionsFound = 0;
    Heuristics.movesEvaluated = 0;
    Heuristics.totalMovesToEvaluate = 0;
    Heuristics.maxDepth = state.MAX_DEPTH;
   

    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
    Heuristics.totalMoves = possibleMoves.size();
    Heuristics.totalMovesToEvaluate += possibleMoves.size();
    //Shuffle Moves to add randomness
    shuffleMoves(possibleMoves);

    //Sort Moves
    //SortMoves()(possibleMoves, board, state.EngineColor);

    auto start = std::chrono::high_resolution_clock::now();
    for (const std::string& move : possibleMoves) {
        if (state.terminateSearch) {
            std::cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << std::endl;
            state.terminateSearch = false;
            break;
        }
        ChessBoard tempBoard(board);
        std::pair<int, int> Indices = ConvertNotation()(move);
        tempBoard.MakeMove(Indices.first, Indices.second);


        int score = Minimax(tempBoard, state.MAX_DEPTH, -infinity, infinity, std::chrono::high_resolution_clock::now(), (state.EngineColor == White) ? Black : White);

        //DebugItem()(move, score);
        Heuristics.movesEvaluated++;
        if (score >= bestScore) {
            bestScore = score;
            bestMove = move;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        Heuristics.TimeTaken = static_cast<float>(duration.count())/1000; //Converted To seconds
        Heuristics.Speed = Heuristics.NumberofMovesLookedAhead / static_cast<float>(Heuristics.TimeTaken);
    }
    

    possibleMoves.clear();

    return bestMove;
}

//int ChessEngine::Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time, int currentPlayer) {
//    Heuristics.NumberofMovesLookedAhead++;
//
//    //board.DrawChessPiece();
//
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - time);
//    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds
//   
//
//    if (Currenttime > state.timeLimit && state.timeLimit != 0) return Evaluate(board, currentPlayer);
//
//    if (state.terminateSearch) return 0;
//
//    uint64_t Boardhash;
//    if (state.useTranspositions) {
//        Boardhash = state.transpostionTable.computeHash(board);
//        if (state.transpostionTable.isValuePresent(Boardhash)) {
//            auto stored = state.transpostionTable.lookupTranspositionTable(Boardhash);
//            if (stored.second >= depth) {
//                Heuristics.TranspositionsFound++;
//                return static_cast<int>(stored.first);
//            }
//        }
//    }
//
//    if (board.isCheckmate(board, currentPlayer)) return (currentPlayer == state.EngineColor) ? -infinity : infinity;
//    if (depth == 0) { return Evaluate(board, board.getCurrentPlayer()); }
//
//    if (currentPlayer == state.EngineColor) {
//        int maxScore = -infinity;
//       
//        std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
//        Heuristics.totalMovesToEvaluate += possibleMoves.size();
//        //Shuffle Moves to add randomness
//        shuffleMoves(possibleMoves);
//        //SortMoves(possibleMoves, board, color);
//
//        for (const std::string& move : possibleMoves) {
//            if (state.terminateSearch) break;
//            std::pair<int, int> Indices = ConvertNotation()(move);
//
//            ChessBoard tempBoard(board);
//            tempBoard.MakeMove(Indices.first, Indices.second);
//            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, White);
//            
//            maxScore = std::max(maxScore, score);
//            alpha = std::max(alpha, score);
//
//            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, maxScore, depth);
//
//            //Alpha-beta pruning
//            if(state.useAlphaBetaPruning){
//                //DebugItem()(beta, alpha, score);
//                if (beta <= alpha) {
//                    Heuristics.BranchesPruned++;
//                    break;
//                }
//            }
//        }
//        possibleMoves.clear();
//        return maxScore;
//    }
//
//    else {
//        int minScore = infinity;
//        
//        std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(White);
//        Heuristics.totalMovesToEvaluate += possibleMoves.size();
//        shuffleMoves(possibleMoves);
//
//        for (const std::string& move : possibleMoves) {
//            if (state.terminateSearch) break;
//
//            std::pair<int, int> Indices = ConvertNotation()(move);
//
//            ChessBoard tempBoard(board);
//            tempBoard.MakeMove(Indices.first, Indices.second);
//
//            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, state.EngineColor);
//
//            minScore = std::min(minScore, score);
//            beta = std::min(beta, score);
//
//            if(state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, minScore, depth);
//
//            //Alpha-beta pruning
//            if (state.useAlphaBetaPruning) {
//                //DebugItem()(beta, alpha, score);
//                if (beta <= alpha) {
//                    Heuristics.BranchesPruned++;
//                    break;
//                }
//            }
//        }
//        possibleMoves.clear();
//        return minScore;
//    }
//}


int ChessEngine::Minimax(ChessBoard &board, int depth, int alpha, int beta, auto time, int currentPlayer) {
    Heuristics.NumberofMovesLookedAhead++;
    int lastPlayer = currentPlayer == Black ? White : Black;
   

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - time);
    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds

    if (Currenttime > state.timeLimit && state.timeLimit != 0) return Evaluate(board, lastPlayer);

    if (state.terminateSearch) return 0;

    

    uint64_t Boardhash;
    if (state.useTranspositions) {
        Boardhash = state.transpostionTable.computeHash(board);
        if (state.transpostionTable.isValuePresent(Boardhash)) {
            auto stored = state.transpostionTable.lookupTranspositionTable(Boardhash);
            if (stored.second >= depth) {
                Heuristics.TranspositionsFound++;
                return stored.first;
            }
        }
    }


   
    if (depth == 0 || board.isCheckmate()) {
        
        if (board.isCheckmate()) {
            return (lastPlayer == state.EngineColor) ? -infinity : infinity;
        }
        return Evaluate(board, lastPlayer);
    }


    

    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(currentPlayer);
    shuffleMoves(possibleMoves);
    Heuristics.totalMovesToEvaluate += possibleMoves.size();

   
    if (currentPlayer == state.EngineColor) {
        int maxScore = -infinity;

        for (const std::string & move : possibleMoves) {
            if (state.terminateSearch) break;
            std::pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);
            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, (state.EngineColor == White) ? Black : White);


            maxScore = std::max(maxScore, score);
            alpha = std::max(alpha, score);

            //DebugItem()(move, score);

            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, maxScore, depth);

            //Alpha-beta pruning
            if (state.useAlphaBetaPruning && beta <= alpha) {
                    Heuristics.BranchesPruned++;
                    break;
            }
        }
        //possibleMoves.clear();
        return maxScore;
    }
    else {
        int minScore = infinity;

        for (const std::string &move : possibleMoves) {
            if (state.terminateSearch) break;
            std::pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);

            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, state.EngineColor);


            minScore = std::min(minScore, score);
            beta = std::min(beta, score);

           //DebugItem()(move, score);

            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, minScore, depth);

            //Alpha-beta pruning
            if (state.useAlphaBetaPruning && beta <= alpha) {
                Heuristics.BranchesPruned++;
                break;
            }
        }
        //possibleMoves.clear();
        return minScore;
    }
}


//int ChessEngine::Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time) {
//    Heuristics.NumberofMovesLookedAhead++;
//
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - time);
//    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds
//
//
//    if (Currenttime > state.timeLimit && state.timeLimit != 0) return Evaluate(board, board.getCurrentPlayer());
//
//    if (state.terminateSearch) return 0;
//
//    uint64_t Boardhash;
//    if (state.useTranspositions) {
//        Boardhash = state.transpostionTable.computeHash(board);
//        if (state.transpostionTable.isValuePresent(Boardhash)) {
//            auto stored = state.transpostionTable.lookupTranspositionTable(Boardhash);
//            if (stored.second >= depth) {
//                Heuristics.TranspositionsFound++;
//                return static_cast<int>(stored.first);
//            }
//        }
//    }
//
//    if (board.isCheckmate(board, board.getCurrentPlayer())) return (board.getCurrentPlayer() == state.EngineColor ? -infinity : infinity);
//    if (depth == 0) { return Evaluate(board, board.getCurrentPlayer()); }
//
//
//    int color = board.getCurrentPlayer();
//    int bestScore = (color == state.EngineColor) ? -infinity : infinity;
//    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(board.getCurrentPlayer());
//    Heuristics.totalMovesToEvaluate += possibleMoves.size();
//    shuffleMoves(possibleMoves);
//
//    for (const std::string& move : possibleMoves) {
//        if (state.terminateSearch) break;
//        std::pair<int, int> Indices = ConvertNotation()(move);
//
//        ChessBoard tempBoard(board);
//        tempBoard.MakeMove(Indices.first, Indices.second);
//        int score = Minimax(tempBoard, depth - 1, alpha, beta, time);
//
//
//        bestScore = (color == state.EngineColor) ? std::max(bestScore, score) : std::min(bestScore, score);
//
//        if (color == state.EngineColor) alpha = std::max(alpha, score);
//        else beta = std::min(beta, score);
//   
//        if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, bestScore, depth);
//
//        //Alpha-beta pruning
//        if (state.useAlphaBetaPruning) {
//                std::cout << beta << " " << alpha << std::endl;
//            if (beta <= alpha) {
//                Heuristics.BranchesPruned++;
//                break;
//            }
//        }
//    }
//    possibleMoves.clear();
//    return bestScore;
//}

//std::string ChessEngine::IterativeDeepening(const ChessBoard& board, int maxDepth) {
//    float bestScore = -infinity;
//    std::string bestMove;
//
//    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
//    //SortMoves(possibleMoves, board, EngineColor);
//    shuffleMoves(possibleMoves);
//
//    for (const std::string& move : possibleMoves) {
//        if (state.terminateSearch) {
//            std::cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << std::endl;
//            state.terminateSearch = false;
//            break;
//        }
//
//        ChessBoard tempBoard(board);
//        std::pair<int, int> Indices = ConvertNotation()(move);
//        tempBoard.MakeMove(Indices.first, Indices.second);
//        float score = Minimax(tempBoard, maxDepth, -infinity, infinity, std::chrono::high_resolution_clock::now());
//
//        if (score >= bestScore) {
//            bestScore = score;
//            bestMove = move;
//        }
//    }
//
//    possibleMoves.clear();
//
//    return bestMove;
//}

int ChessEngine::Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const {

    if (state.terminateSearch) return 0;
    // Evaluate material advantage
    int SelfMaterial = 0, OpponentMeterial = 0;
    // Evaluate positional advantage using PSTs
    int positionalAdvantage = 0;

    for (int i = 0; i < Total_tiles; ++i) {
        ChessPiece* piece = chessboard.board[i];
        if (piece->type != EMPTY) {
            positionalAdvantage += pst.getPSTValue(piece, i, currentPlayerColor);
            
            if (piece->color == currentPlayerColor) {
                SelfMaterial += state.getPieceValue(piece->PieceCode);
                
            }
            else {
                OpponentMeterial += state.getPieceValue(piece->PieceCode);
            }
        }
    }


    
    int materialAdvantage = SelfMaterial - OpponentMeterial;

 
    // Total evaluation is a combination of material, positional, and safety advantages
    return materialAdvantage + positionalAdvantage;
}

void ChessEngine::adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const {


    int kingSquareIndex = chessboard.GetKingIndex(currentPlayerColor);
    int opponentKingSquareIndex = chessboard.GetKingIndex((currentPlayerColor == White) ? Black : White);


    int kingRow = kingSquareIndex / 8;
    int kingCol = kingSquareIndex % 8;
    int opponentKingRow = opponentKingSquareIndex / 8;
    int opponentKingCol = opponentKingSquareIndex % 8;

    // Encourage forcing opponent's king to the corner
    int cornerDist = std::min(std::min(opponentKingRow, 7 - opponentKingRow), std::min(opponentKingCol, 7 - opponentKingCol));
    positionalAdvantage += (currentPlayerColor == state.EngineColor ? 10 : -10) * cornerDist;

    // Encourage bringing own king closer to the opponent's king
    int kingDist = abs(kingRow - opponentKingRow) + abs(kingCol - opponentKingCol);
    positionalAdvantage += (currentPlayerColor == state.EngineColor ? -5 : 5) * kingDist;
}


void ChessEngine::PlayMove(const std::string& move, ChessBoard& board) const {

    std::pair<int, int> Indices = ConvertNotation()(move);
    board.MakeCompleteMove(Indices.first, Indices.second, move);
}

