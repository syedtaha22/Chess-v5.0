#include "../../headers/Engine/ChessEngine.h"



ChessEngine::ChessEngine(int Color, bool loadSettings) {
    if(loadSettings) state.LoadSavedSettings();
    state.setEngineColor(Color);
}

void ChessEngine::shuffleMoves(std::vector<std::string>& possibleMoves) {
    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine(static_cast<unsigned int>(seed)));
}

std::string ChessEngine::GenerateMove(const ChessBoard& board) {
    //std::unique_ptr<Timer> timer = std::make_unique<Timer>();
    int bestScore = -infinity;
    
    std::string bestMove;
    
    Heuristics.NumberofMovesLookedAhead = 0;
    Heuristics.TranspositionsFound = 0;
    Heuristics.movesEvaluated = 0;
    Heuristics.totalMovesToEvaluate = 0;
    Heuristics.maxDepth = state.getDepth();
   

    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.GetEngineColor());
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


        int score = Minimax(tempBoard, state.getDepth(), -infinity, infinity, std::chrono::high_resolution_clock::now(), White);

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

int ChessEngine::Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time, int currentPlayer) {
    Heuristics.NumberofMovesLookedAhead++;
    int LastPlayer = currentPlayer == White ? Black : White;

    //board.DrawChessPiece();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - time);
    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds


    if (Currenttime > state.getTimeLimit() && state.getTimeLimit() != 0) return Evaluate(board, currentPlayer);

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

   
    if (board.isCheckmate(currentPlayer)) return infinity;
    if (depth == 0) {
        return Evaluate(board, currentPlayer);

    }

    std::vector<std::string> possibleMoves = board.GetAllPossibleMovesInChessNotation(currentPlayer);
    Heuristics.totalMovesToEvaluate += possibleMoves.size();
    //Shuffle Moves to add randomness
    shuffleMoves(possibleMoves);

    if (currentPlayer == state.GetEngineColor()) {
        int maxScore = -infinity;

        for (const std::string& move : possibleMoves) {
            if (state.terminateSearch) break;
            std::pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);
            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, LastPlayer);

            maxScore = std::max(maxScore, score);
            alpha = std::max(alpha, score);

            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, maxScore, depth);

            //DebugItem()(score, maxScore);

            //Alpha-beta pruning
            if (state.useAlphaBetaPruning) {
                if (beta <= alpha) {
                    Heuristics.BranchesPruned++;
                    break;
                }
            }
        }
        possibleMoves.clear();
        return maxScore;
    }

    else {
        int minScore = infinity;

        for (const std::string& move : possibleMoves) {
            if (state.terminateSearch) break;

            std::pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);

            int score = Minimax(tempBoard, depth - 1, alpha, beta, time, LastPlayer);

            minScore = std::min(minScore, score);
            beta = std::min(beta, score);

            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, minScore, depth);

            //DebugItem()(score, minScore);

            //Alpha-beta pruning
            if (state.useAlphaBetaPruning) {
                if (beta <= alpha) {
                    Heuristics.BranchesPruned++;
                    break;
                }
            }
        }
        possibleMoves.clear();
        return minScore;
    }
}

int ChessEngine::Evaluate(const ChessBoard& chessboard, int currentPlayerColor) const {
    if (state.terminateSearch) return 0;

    // Evaluate material advantage

    int SelfMaterial = 0;
    int OpponentMeterial = 0;
    int positionalAdvantage = 0;

    for (int i = 0; i < Total_tiles; ++i) {
        ChessPiece* piece = chessboard.board[i];
        int pieceValue = PST.getPSTValue(piece, i, currentPlayerColor);
        positionalAdvantage += pieceValue;
        if (piece->type != EMPTY) {
            if (piece->color == currentPlayerColor) {
                SelfMaterial += state.getPieceValue(piece->type);
            }
            else {
                OpponentMeterial += state.getPieceValue(piece->type);
            }
        }
    }
    int materialAdvantage = SelfMaterial - OpponentMeterial;

    if ((SelfMaterial < 120 && OpponentMeterial < 120)) {
        adjustEndgamePositionalAdvantage(chessboard, currentPlayerColor, positionalAdvantage);
    }

    //DebugItem()(SelfMaterial, OpponentMeterial, positionalAdvantage, materialAdvantage);

    // Total evaluation is a combination of material, positional, and safety advantages
    return materialAdvantage + positionalAdvantage;
}

void ChessEngine::adjustEndgamePositionalAdvantage(const ChessBoard& chessboard, int currentPlayerColor, int& positionalAdvantage) const {

    int kingSquareIndex = chessboard.GetKingIndex(currentPlayerColor);
    int opponentKingSquareIndex = chessboard.GetKingIndex((currentPlayerColor == White) ? Black : White);


    //Not using ConvertNotation() here for simplicity
    int kingRow = kingSquareIndex / 8;
    int kingCol = kingSquareIndex % 8;
    int opponentKingRow = opponentKingSquareIndex / 8;
    int opponentKingCol = opponentKingSquareIndex % 8;

    // Encourage forcing opponent's king to the corner
    int cornerDist = std::min(std::min(opponentKingRow, 7 - opponentKingRow), std::min(opponentKingCol, 7 - opponentKingCol));
    positionalAdvantage += (currentPlayerColor == state.GetEngineColor() ? 10 : -10) * cornerDist;

    // Encourage bringing own king closer to the opponent's king
    int kingDist = abs(kingRow - opponentKingRow) + abs(kingCol - opponentKingCol);
    positionalAdvantage += (currentPlayerColor == state.GetEngineColor() ? -5 : 5) * kingDist;

}

void ChessEngine::PlayMove(const std::string& move, ChessBoard& board) const {

    std::pair<int, int> Indices = ConvertNotation()(move);
    board.MakeCompleteMove(Indices.first, Indices.second, move);
}