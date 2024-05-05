#include "../../headers/Engine/ChessEngine.h"



ChessEngine::ChessEngine(int Color) {
    state.EngineColor = Color;
    state.initPieceValues();
}

void ChessEngine::shuffleMoves(vector<string>& possibleMoves) {
    long long seed = system_clock::now().time_since_epoch().count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(static_cast<unsigned int>(seed)));
}

string ChessEngine::GenerateMove(const ChessBoard& board) {
    int bestScore = -infinity;
    
    string bestMove;
    
    Heuristics.NumberofMovesLookedAhead = 0;
    Heuristics.TranspositionsFound = 0;
    Heuristics.movesEvaluated = 0;
    Heuristics.totalMovesToEvaluate = 0;
    Heuristics.maxDepth = state.MAX_DEPTH;
   

    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
    Heuristics.totalMoves = possibleMoves.size();
    Heuristics.totalMovesToEvaluate += possibleMoves.size();
    //Shuffle Moves to add randomness
    shuffleMoves(possibleMoves);
    //SortMoves(possibleMoves, board, EngineColor);

    auto start = high_resolution_clock::now();
    for (const string& move : possibleMoves) {
        if (state.terminateSearch) {
            cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
            state.terminateSearch = false;
            break;
        }
        ChessBoard tempBoard(board);
        pair<int, int> Indices = ConvertNotation()(move);
        tempBoard.MakeMove(Indices.first, Indices.second);


        int score = Minimax(tempBoard, state.MAX_DEPTH, -infinity, infinity, high_resolution_clock::now());
        Heuristics.movesEvaluated++;
        if (score >= bestScore) {
            bestScore = score;
            bestMove = move;
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        Heuristics.TimeTaken = static_cast<float>(duration.count())/1000; //Converted To seconds
        Heuristics.Speed = Heuristics.NumberofMovesLookedAhead / static_cast<float>(Heuristics.TimeTaken);
    }
    

    possibleMoves.clear();

    return bestMove;
}

int ChessEngine::Minimax(ChessBoard& board, int depth, int alpha, int beta, auto time) {
    Heuristics.NumberofMovesLookedAhead++;
   

    //board.DrawChessPiece();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - time);
    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds
   

    if (Currenttime > state.timeLimit && state.timeLimit != 0) return Evaluate(board, board.getCurrentPlayer());

    if (state.terminateSearch) return 0;

    uint64_t Boardhash;
    if (state.useTranspositions) {
        Boardhash = state.transpostionTable.computeHash(board);
        if (state.transpostionTable.isValuePresent(Boardhash)) {
            auto stored = state.transpostionTable.lookupTranspositionTable(Boardhash);
            if (stored.second >= depth) {
                Heuristics.TranspositionsFound++;
                return static_cast<int>(stored.first);
            }
        }
    }

    if (board.isCheckmate(board, board.getCurrentPlayer())) return (board.getCurrentPlayer() == state.EngineColor ? -infinity : infinity);
    if (depth == 0) {
        return Evaluate(board, board.getCurrentPlayer());

    }

    if (board.getCurrentPlayer() == state.EngineColor) {
        int maxScore = -infinity;
       
        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
        Heuristics.totalMovesToEvaluate += possibleMoves.size();
        //Shuffle Moves to add randomness
        shuffleMoves(possibleMoves);
        //SortMoves(possibleMoves, board, color);

        for (const string& move : possibleMoves) {
            if (state.terminateSearch) break;
            pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);
            int score = Minimax(tempBoard, depth - 1, alpha, beta, time);
            
            maxScore = max(maxScore, score);
            alpha = max(alpha, score);

            if (state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, maxScore, depth);

            //Alpha-beta pruning
            if(state.useAlphaBetaPruning){
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
        
        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(board.getCurrentPlayer());
        Heuristics.totalMovesToEvaluate += possibleMoves.size();
        shuffleMoves(possibleMoves);

        for (const string& move : possibleMoves) {
            if (state.terminateSearch) break;

            pair<int, int> Indices = ConvertNotation()(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);

            int score = Minimax(tempBoard, depth - 1, alpha, beta, time);

            minScore = min(minScore, score);
            beta = min(beta, score);

            if(state.useTranspositions) state.transpostionTable.storeTranspositionTable(Boardhash, minScore, depth);

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

string ChessEngine::IterativeDeepening(const ChessBoard& board, int maxDepth) {
    int bestScore = -infinity;
    string bestMove;

    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(state.EngineColor);
    //SortMoves(possibleMoves, board, EngineColor);
    shuffleMoves(possibleMoves);

    for (const string& move : possibleMoves) {
        if (state.terminateSearch) {
            cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
            state.terminateSearch = false;
            break;
        }

        ChessBoard tempBoard(board);
        pair<int, int> Indices = ConvertNotation()(move);
        tempBoard.MakeMove(Indices.first, Indices.second);
        int score = Minimax(tempBoard, maxDepth, -infinity, infinity, high_resolution_clock::now());

        if (score >= bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    possibleMoves.clear();

    return bestMove;
}

int ChessEngine::Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const {

    if (state.terminateSearch) return 0;
    // Evaluate material advantage
    int SelfMaterial = 0, OpponentMeterial = 0;
    for (int i = 0; i < Total_tiles; ++i) {
        ChessPiece* piece = chessboard.board[i];
        if (piece->type != EMPTY) {
            if (piece->color == state.EngineColor) {
                SelfMaterial += state.getPieceValue(piece->PieceCode);
            }
            else {
                OpponentMeterial += state.getPieceValue(piece->PieceCode);
            }
        }
    }
    int materialAdvantage = (currentPlayerColor == White) ? (SelfMaterial - OpponentMeterial) : (OpponentMeterial - SelfMaterial);

    // Evaluate positional advantage using PSTs
    int positionalAdvantage = 0;
    for (int i = 0; i < Total_tiles; ++i) {
        ChessPiece* piece = chessboard.board[i];
        if (piece->type != EMPTY) {
            int pieceValue = getPSTValue(piece, i, currentPlayerColor);
            positionalAdvantage += pieceValue;
        }
    }
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
    int cornerDist = min(min(opponentKingRow, 7 - opponentKingRow), min(opponentKingCol, 7 - opponentKingCol));
    positionalAdvantage += (currentPlayerColor == state.EngineColor ? 10 : -10) * cornerDist;

    // Encourage bringing own king closer to the opponent's king
    int kingDist = abs(kingRow - opponentKingRow) + abs(kingCol - opponentKingCol);
    positionalAdvantage += (currentPlayerColor == state.EngineColor ? -5 : 5) * kingDist;
}

int ChessEngine::getPSTValue(ChessPiece* piece, int squareIndex, char currentPlayerColor) const {
    if (state.terminateSearch) return 0;
    const int* piecePST = nullptr;

    switch (piece->type) {
    case PAWN:
        piecePST = (currentPlayerColor == Black) ? InvertTable(PawnPST) : PawnPST;
        break;
    case KNIGHT:
        piecePST = (currentPlayerColor == Black) ? InvertTable(KnightsPST) : KnightsPST;
        break;
    case BISHOP:
        piecePST = (currentPlayerColor == Black) ? InvertTable(BishopsPST) : BishopsPST;
        break;
    case ROOK:
        piecePST = (currentPlayerColor == Black) ? InvertTable(RooksPST) : RooksPST;
        break;
    case QUEEN:
        piecePST = (currentPlayerColor == Black) ? InvertTable(QueensPST) : QueensPST;
        break;
    case KING:
        piecePST = (currentPlayerColor == Black) ? InvertTable(KingPST) : KingPST;
        break;
    }

    // Ensure that the piece type index is within bounds
    if (piecePST != nullptr) {
        return piecePST[squareIndex];
    }
    else {
        // Handle unknown piece types or other errors
        return 0; // Or handle the error in an appropriate way
    }
}

void ChessEngine::PlayMove(const string& move, ChessBoard& board) const {

    pair<int, int> Indices = ConvertNotation()(move);
    board.MakeCompleteMove(Indices.first, Indices.second, move);
}

int* ChessEngine::InvertTable(const int* originalArray) const {
    int* invertedArray = new int[Total_tiles]; // Create an array of size 64

    for (int i = 0; i < Total_tiles; i++) {
        invertedArray[i] = originalArray[63 - i];
    }

    return invertedArray;
}

bool ChessEngine::CheckAfterMove(const string& move, const ChessBoard& board, int color) const {
    ChessBoard tempBoard(board); 
    pair<int, int> indices = ConvertNotation()(move);
    tempBoard.MakeMove(indices.first, indices.second);

    // Check if the move puts the opponent's king in check
    return tempBoard.isCheck(tempBoard, (color == White ? White : Black), "Engine: Check after move");
}

bool ChessEngine::IsCaptureMove(const string& move, const ChessBoard board) const {
    pair<int, int> indices = ConvertNotation()(move);
    ChessPiece* targetPiece = board.GetPieceAtPosition(indices.second);

    // Check if the target piece is an opponent's piece
    return targetPiece->color != board.GetPieceAtPosition(indices.first)->color && targetPiece->color != EMPTY;
}

void ChessEngine::DisplayMoves(vector<string>& moves) {
    for (const auto& move : moves) {
        cout << move << " ";
    }
    cout << endl;
}