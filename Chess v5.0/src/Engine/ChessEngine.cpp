#include "../../headers/Engine/ChessEngine.h"



ChessEngine::ChessEngine(int Color) : EngineColor(Color) {
    engineEloRating = readEloFromFile();
    NumberofMovesLookedAhead = 0;
    TimeTakenForSearch = 0;
    NumberOfBranchesPruned = 0;
    NumberOfTranspositionsFound = 0;
    terminateSearch = false;
    startSearch = false;
    EngineSpeed = 0;
    totalMoves = 0;
    movesEvaluated = 0;
    totalMovesToEvaluate = 0;


    

}

void ChessEngine::setEngineColor(int color) {
    EngineColor = color;
}

void ChessEngine::shuffleMoves(vector<string>& possibleMoves) {
    long long seed = system_clock::now().time_since_epoch().count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(static_cast<unsigned int>(seed)));
}

void ChessEngine::SetDepth(int NewDepth) {
    MAX_DEPTH = NewDepth;
}

string ChessEngine::GenerateMove(const ChessBoard& board) {
    int bestScore = -infinity;
    
    string bestMove;
    
    NumberofMovesLookedAhead = 0;
    NumberOfTranspositionsFound = 0;
    movesEvaluated = 0;
    totalMovesToEvaluate = 0;

   

    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(EngineColor);
    totalMoves = static_cast<int>(possibleMoves.size());
    totalMovesToEvaluate += static_cast<int>(possibleMoves.size());
    //Shuffle Moves to add randomness
    shuffleMoves(possibleMoves);
    //SortMoves(possibleMoves, board, EngineColor);

    auto start = high_resolution_clock::now();
    for (const string& move : possibleMoves) {
        if (terminateSearch) {
            cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
            terminateSearch = false;
            break;
        }
        ChessBoard tempBoard(board);
        pair<int, int> Indices = convertChessNotationToIndices(move);
        tempBoard.MakeMove(Indices.first, Indices.second);


        int score = Minimax(tempBoard, MAX_DEPTH, -infinity, infinity, false, EngineColor == White ? Black : White, high_resolution_clock::now());
        movesEvaluated++;
        if (score >= bestScore) {
            bestScore = score;
            bestMove = move;
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        TimeTakenForSearch = static_cast<float>(duration.count())/1000; //Converted To seconds
        EngineSpeed = NumberofMovesLookedAhead / static_cast<float>(TimeTakenForSearch);
    }
    

    possibleMoves.clear();

    return bestMove;
}

int ChessEngine::Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color, auto time) {
    NumberofMovesLookedAhead++;

    //board.DrawChessPiece();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - time);
    auto Currenttime = static_cast<float>(duration.count()) / 1000; //Converted To seconds
   

    if (Currenttime > timeLimit && timeLimit != 0) return Evaluate(board, color);

    if (terminateSearch) return 0;

    uint64_t Boardhash;
    if (useTranspositions) {
        Boardhash = transpostionTable.computeHash(board);
        if (transpostionTable.isValuePresent(Boardhash)) {
            auto stored = transpostionTable.lookupTranspositionTable(Boardhash);
            if (stored.second >= depth) {
                NumberOfTranspositionsFound++;
                return static_cast<int>(stored.first);
            }
        }
    }

    if (board.isCheckmate(board, color)) return (maximizingPlayer ? -infinity : infinity);
    if (depth == 0) return Evaluate(board, color);



    if (maximizingPlayer) {
        int maxScore = -infinity;
       
        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
        totalMovesToEvaluate += static_cast<int>(possibleMoves.size());
        //Shuffle Moves to add randomness
        shuffleMoves(possibleMoves);
        //SortMoves(possibleMoves, board, color);

        for (const string& move : possibleMoves) {
            if (terminateSearch) break;
            pair<int, int> Indices = convertChessNotationToIndices(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);
            int score = Minimax(tempBoard, depth - 1, alpha, beta, false, color == White ? Black : White, time);
            


            maxScore = max(maxScore, score);
            alpha = max(alpha, score);

            if (useTranspositions) transpostionTable.storeTranspositionTable(Boardhash, maxScore, depth);

            //Alpha-beta pruning
            if(useAlphaBetaPruning){
                if (beta <= alpha) {
                    NumberOfBranchesPruned++;
                    break;
                }
            }
        }
        possibleMoves.clear();
        return maxScore;
    }

    else {
        int minScore = infinity;
        
        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
        totalMovesToEvaluate += static_cast<int>(possibleMoves.size());
        //SortMoves(possibleMoves, board, color);

        //Shuffle Moves to add randomness
        shuffleMoves(possibleMoves);


        for (const string& move : possibleMoves) {
            if (terminateSearch) break;

            pair<int, int> Indices = convertChessNotationToIndices(move);

            ChessBoard tempBoard(board);
            tempBoard.MakeMove(Indices.first, Indices.second);

            int score = Minimax(tempBoard, depth - 1, alpha, beta, true, color == White ? Black : White, time);


            minScore = min(minScore, score);
            beta = min(beta, score);


            if(useTranspositions) transpostionTable.storeTranspositionTable(Boardhash, minScore, depth);

            //Alpha-beta pruning
            if (useAlphaBetaPruning) {
                if (beta <= alpha) {
                    NumberOfBranchesPruned++;
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

    vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(EngineColor);
    //SortMoves(possibleMoves, board, EngineColor);
    shuffleMoves(possibleMoves);

    for (const string& move : possibleMoves) {
        if (terminateSearch) {
            cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
            terminateSearch = false;
            break;
        }

        ChessBoard tempBoard(board);
        pair<int, int> Indices = convertChessNotationToIndices(move);
        tempBoard.MakeMove(Indices.first, Indices.second);
        int score = Minimax(tempBoard, maxDepth, -infinity, infinity, false, EngineColor == White ? Black : White, high_resolution_clock::now());

        if (score >= bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    possibleMoves.clear();

    return bestMove;
}

int ChessEngine::Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const {

    if (terminateSearch) return 0;
    unordered_map<char, int> pieceValues;
    // Piece values for evaluation
    if (EngineColor == White) {
        pieceValues = {
            {White | PAWN,  100}, {White | KNIGHT,  320}, {White | BISHOP,  330}, {White | ROOK,  500}, {White | QUEEN,  900}, {White | KING,  infinity},
            {Black | PAWN, -100}, {Black | KNIGHT, -320}, {Black | BISHOP, -330}, {Black | ROOK, -500}, {Black | QUEEN, -900}, {Black | KING, -infinity} };
    }
    else {
        pieceValues = {
            {White | PAWN, -100}, {White | KNIGHT, -320}, {White | BISHOP, -330}, {White | ROOK, -500}, {White | QUEEN, -900}, {White | KING, -infinity},
            {Black | PAWN,  100}, {Black | KNIGHT,  320}, {Black | BISHOP,  330}, {Black | ROOK,  500}, {Black | QUEEN,  900}, {Black | KING,  infinity} };
    }

    // Evaluate material advantage
    int SelfMaterial = 0, OpponentMeterial = 0;
    for (int i = 0; i < Total_tiles; ++i) {
        ChessPiece* piece = chessboard.board[i];
        if (piece->type != EMPTY) {
            if (piece->color == EngineColor) {
                SelfMaterial += pieceValues[piece->PieceCode];
            }
            else {
                OpponentMeterial += pieceValues[piece->PieceCode];
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
    positionalAdvantage += (currentPlayerColor == EngineColor ? 10 : -10) * cornerDist;

    // Encourage bringing own king closer to the opponent's king
    int kingDist = abs(kingRow - opponentKingRow) + abs(kingCol - opponentKingCol);
    positionalAdvantage += (currentPlayerColor == EngineColor ? -5 : 5) * kingDist;
}

int ChessEngine::getPSTValue(ChessPiece* piece, int squareIndex, char currentPlayerColor) const {
    if (terminateSearch) return 0;
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

pair<int, int> ChessEngine::convertChessNotationToIndices(const string& move) const {
    int fromCol = move[0] - 'a';
    int fromRow = 7 - (move[1] - '1');
    int toCol = move[2] - 'a';
    int toRow = 7 - (move[3] - '1');
    int fromTile = fromRow * 8 + fromCol;
    int toTile = toRow * 8 + toCol;
    return make_pair(fromTile, toTile);
}

void ChessEngine::PlayMove(const string& move, ChessBoard& board) const {

    pair<int, int> Indices = convertChessNotationToIndices(move);
    board.MakeCompleteMove(Indices.first, Indices.second, move);
}

int* ChessEngine::InvertTable(const int* originalArray) const {
    int* invertedArray = new int[Total_tiles]; // Create an array of size 64

    for (int i = 0; i < Total_tiles; i++) {
        invertedArray[i] = originalArray[63 - i];
    }

    return invertedArray;
}

int ChessEngine::readEloFromFile() {
    string folder = "Data";
    if (!filesystem::exists(folder)) {
        cerr << "Folder " << folder << " does not exist. Using default Elo rating." << endl;
        return engineEloRating;
    }

    ifstream inputFile;
    string filename = "Data/Engine'sELO.txt";
    inputFile.open(filename);

    int elo = engineEloRating; 

    if (inputFile.is_open()) {
        inputFile >> elo;
        inputFile.close();
        cout << "Elo rating read from " << filename << ": " << elo << endl;
    }
    else {
        cout << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;

    }

    return elo;
}

void ChessEngine::saveEloToFile() const {
    string folder = "Data";

    if (!filesystem::is_directory(folder)) {
        filesystem::create_directory(folder);
    }

    ofstream outputFile;
    string filename = "Data/Engine'sELO.txt";
    outputFile.open(filename);

    int defaultElo = 500;

    if (outputFile.is_open()) {

        outputFile << engineEloRating << endl;
        outputFile.close();
        cout << "Elo rating saved to " << filename << endl;
    }
    else {
        cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
        outputFile.open(filename);
        if (outputFile.is_open()) {
            outputFile << defaultElo << std::endl;
            outputFile.close();
            cout << "New file created with default Elo rating: " << defaultElo << endl;
        }
        else {
            cerr << "Unable to create file " << filename << " for writing." << endl;
        }
    }
}

void ChessEngine::Reset() {
    TimeTakenForSearch = 0;
    NumberofMovesLookedAhead = 0;
    //terminateSearch = false;
    startSearch = false;
    NumberOfBranchesPruned = 0;
    NumberOfTranspositionsFound = 0;
}

// Function to check if a move results in a check
bool ChessEngine::CheckAfterMove(const string& move, const ChessBoard& board, int color) const {
    ChessBoard tempBoard(board); 
    pair<int, int> indices = convertChessNotationToIndices(move);
    tempBoard.MakeMove(indices.first, indices.second);

    // Check if the move puts the opponent's king in check
    return tempBoard.isCheck(tempBoard, (color == White ? White : Black), "Engine: Check after move");
}

bool ChessEngine::IsCaptureMove(const string& move, const ChessBoard board) const {
    pair<int, int> indices = convertChessNotationToIndices(move);
    ChessPiece* targetPiece = board.GetPieceAtPosition(indices.second);

    // Check if the target piece is an opponent's piece
    return targetPiece->color != board.GetPieceAtPosition(indices.first)->color && targetPiece->color != EMPTY;
}

double ChessEngine::getSizeOfTranspositionTable() const {
    return transpostionTable.SizeOfTranspostionTable;
}

void ChessEngine::StartSearch() {
    startSearch = true;
}

void ChessEngine::StopSearching() {
    startSearch = false; // Set the stop flag to stop the search
}

void ChessEngine::TerminateSearch() {
    terminateSearch = true;
}

bool ChessEngine::isSearchStarted() const {

    return startSearch;
}

void ChessEngine::DisplayMoves(vector<string>& moves) {
    for (const auto& move : moves) {
        cout << move << " ";
    }
    cout << endl;
}


void ChessEngine::SaveTranspositionTable() {
    transpostionTable.saveTranspositionTableToFile("TranspositionTables.txt");
}

int ChessEngine::getDepth() const {
    return MAX_DEPTH;
}



