#include "../../headers/Board/ChessBoard.h"


ChessBoard::ChessBoard() {
    for (int i = 0; i < Total_tiles; ++i) {
        board[i] = new ChessPiece();
    }

}

ChessBoard::ChessBoard(const ChessBoard& other) {
    // Allocate memory for the new board
    for (int i = 0; i < Total_tiles; ++i) {
        board[i] = new ChessPiece(*(other.board[i]));
    }
    state = other.state;
}

void ChessBoard::AddMoveToHistory(std::string move) {
    state.moveHistory.push_back(move);
}

void ChessBoard::promotePawn(int toTile) {
    bool PawnPromoted = false;

    // Dynamic Pawn Promotion Under Development...

    //Promote Pawn to Queen
    delete board[toTile];
    board[toTile] = new Queen(state.currentPlayerIsWhite ? White : Black);
    board[toTile]->AssignTextures();
    SetPiecePositions();
}

std::vector<int> ChessBoard::FilterValidMoves(int fromIndex, std::vector<int> possibleMoves) const {
    //This function filters out any moves that leave the king in check
    ChessPiece* piece = board[fromIndex];

    std::vector<int> filteredMoves;
    for (int toTile : possibleMoves) {
        
        ChessBoard tempBoard(*this);
        tempBoard.MakeMove(fromIndex, toTile);
        if (!tempBoard.isCheck(tempBoard, piece->color, "Board: Filter Valid Moves")) {
            filteredMoves.push_back(toTile);
        }
    }
    return filteredMoves;
}

int ChessBoard::calculatePlayerScore(int playerColor) const {
    int totalScore = 0;
    for (int i = 0; i < Total_tiles; i++) {
        if (board[i]->color == playerColor) {
            totalScore += board[i]->getScore();
        }
    }
    return totalScore;
}

void ChessBoard::initializeBoardFromFEN(const std::string& fen, bool loadTextures) {
    //std::cout << "Starting Initialising...." << std::endl;
    state.moveHistory.clear();
    state.MoveIndices = { -1,-1 };
    int row = 0, col = 0;
    // Parse the FEN std::string
    for (char symbol : fen) {
        if (symbol == ' ') {
            break;  // Stop parsing after the board section
        }
        if (symbol == '/') {
            row++;
            col = 0;
        }
        else if (isdigit(symbol)) {
            // Fill empty squares --> OtherWise we might get unexpected pieces

            //Convert char Number to Interger
            int numEmptySquares = symbol - '0'; 
            for (int i = 0; i < numEmptySquares; i++) {
                board[row * 8 + col] = new ChessPiece(EMPTY, EMPTY);
                col++;
            }
        }
        else {
            // Set piece based on FEN character

            int pieceColor = (isupper(symbol) ? White : Black);
            switch (toupper(symbol)) {
            case 'K': board[row * 8 + col] = new King(pieceColor); break;
            case 'P': board[row * 8 + col] = new Pawn(pieceColor); break;
            case 'N': board[row * 8 + col] = new Knight(pieceColor); break;
            case 'B': board[row * 8 + col] = new Bishop(pieceColor); break;
            case 'R': board[row * 8 + col] = new Rook(pieceColor);; break;
            case 'Q': board[row * 8 + col] = new Queen(pieceColor); break;
            default: break;
            }
            col++;
        }
    }
    // Set the current player based on FEN
    state.currentPlayerIsWhite = (fen.find("w") != std::string::npos);

    //SetCastlingRights
    int WhiteKingIndex = GetKingIndex(White);
    int BlackKingIndex = GetKingIndex(Black);
    size_t castlingRightsPos = fen.find_last_of(" ");
    if (castlingRightsPos != std::string::npos && castlingRightsPos + 1 < fen.length()) {
        std::string castlingRights = fen.substr(castlingRightsPos + 1);
        for (char c : castlingRights) {
            if (c == 'K') {
                board[WhiteKingIndex]->canCastleKingSide = true;
            }
            else if (c == 'Q') {
                board[WhiteKingIndex]->canCastleQueenSide = true;
            }
            else if (c == 'k') {
                board[BlackKingIndex]->canCastleKingSide = true;
            }
            else if (c == 'q') {
                board[BlackKingIndex]->canCastleQueenSide = true;
            }
        }
    }

    if(loadTextures) LoadTextures();
    // Set piece positions
    SetPiecePositions();
    //std::cout << "Initialised\n";
}

int ChessBoard::GetKingIndex(const int& playercolor) const {
    for (int index = 0; index < Total_tiles; index++) {
        if (board[index]->type == KING && board[index]->color == playercolor) {
            return index;
        }
    }
    // Exit Game if King Cannot be found
    std::cout << "\n\nGetKingIndex: KING(" << playercolor << ") not Found\n\n";
    exit(404);

}

void ChessBoard::saveCurrentFENtofile(std::string fileName) const {
    // Get the path to the user's appdata folder
    const char* appdataFolder = getenv("APPDATA");
    if (appdataFolder == nullptr) {
        std::cerr << "Error: Unable to retrieve user's appdata folder path." << std::endl;
        return;
    }
    // Construct the full path to the folder in the appdata folder
    std::string folder = std::string(appdataFolder) + "\\ChessData";
    if (!std::filesystem::is_directory(folder))
        std::filesystem::create_directory(folder);

    // Construct the full path to the file within the appdata folder
    std::string filePath = folder + "\\" + fileName;

    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    // Generate FEN std::string from current board state
    std::string fen = GetCurrentFEN();

    file << fen << std::endl;
    file.close();
    std::cout << "Saved FEN std::string to: " << filePath << std::endl;
}

std::string ChessBoard::GetCurrentFEN() const {
    std::string fen;
    for (int row = 0; row < 8; row++) {
        int emptyCount = 0;
        for (int col = 0; col < 8; col++) {
            int index = row * 8 + col;
            if (board[index]->type == EMPTY) {
                emptyCount++;
            }
            else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                switch (board[index]->type) {
                case KING: fen += (board[index]->color == White) ? 'K' : 'k'; break;
                case PAWN: fen += (board[index]->color == White) ? 'P' : 'p'; break;
                case ROOK: fen += (board[index]->color == White) ? 'R' : 'r'; break;
                case QUEEN: fen += (board[index]->color == White) ? 'Q' : 'q'; break;
                case KNIGHT: fen += (board[index]->color == White) ? 'N' : 'n'; break;
                case BISHOP: fen += (board[index]->color == White) ? 'B' : 'b'; break;
                default:
                    // Handle unexpected piece type
                    std::cerr << "Error: Unexpected piece type encountered." << std::endl;
                    break;
                }
            }
        }
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (row < 7) {
            fen += '/';
        }
    }
    fen += (state.currentPlayerIsWhite ? " w " : " b ");
    //Add Castling Rights......

    return fen;



}

void ChessBoard::InitializeDefaultBoard() {
    std::string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    initializeBoardFromFEN(DefaultFEN, true);
}

void ChessBoard::initializeBoard() {

    //This is An Older implementation, Manually Placing Chess Pieces On board

    board[0] = new Rook(ROOK, Black);
    board[1] = new Knight(KNIGHT, Black);
    board[2] = new Bishop(BISHOP, Black);
    board[3] = new Queen(QUEEN, Black);
    board[4] = new King(KING, Black);
    board[5] = new Bishop(BISHOP, Black);
    board[6] = new Knight(KNIGHT, Black);
    board[7] = new Rook(ROOK, Black);
    for (int i = 8; i <= 15; i++) {
        board[i] = new Pawn(PAWN, Black);
    }

    // Empty spaces in the middle
    for (int i = 16; i <= 47; i++) {
        board[i] = new ChessPiece(EMPTY, EMPTY);

    }

    // White pieces
    for (int i = 48; i <= 55; i++) {
        board[i] = new Pawn(PAWN, White);
    }
    board[56] = new Rook(ROOK, White);
    board[57] = new Knight(KNIGHT, White);
    board[58] = new Bishop(BISHOP, White);
    board[59] = new Queen(QUEEN, White);
    board[60] = new King(KING, White);
    board[61] = new Bishop(BISHOP, White);
    board[62] = new Knight(KNIGHT, White);
    board[63] = new Rook(ROOK, White);
    SetPiecePositions();
}

//void ChessBoard::DisplayBoard() const {
//    //Display Board on Terminal. For Debugging Purposes
//
//    std::cout << "8 ";
//    for (int i = 0; i < Total_tiles; i++) {
//        // std::cout << (i/8) + 1  << " ";
//        std::cout << "[" << board[i]->type << "]";
//        if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles) {
//            std::cout << std::endl;
//            std::cout << 8 - (i + 1) / 8 << " ";
//        }
//
//        // std::cout << std::endl;
//    }
//    std::cout << std::endl;
//    std::cout << "   a  b  c  d  e  f  g  h" << std::endl;
//}

void ChessBoard::SetPiecePositions() {
    //Set Piece Positions, on graphical Board everytime Board is changed
    for (int index = 0; index < Total_tiles; index++) {
        std::pair<int, int> PieceCoords = ConvertNotation()(index);
        board[index]->rectangle.x = BoardOffsetX + static_cast<float>((abs((state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize);
        board[index]->rectangle.y = BoardOffsetY + static_cast<float>((abs((state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize);
    }
}

int ChessBoard::getTileIndex(float x, float y, int tileSize) {

    int file = static_cast<int>(x / tileSize);
    int rank = static_cast<int>(y / tileSize);
    int index = rank * 8 + file;

    return index;
}

void ChessBoard::UpdateChessPiece(ChessPiece* piece, int InitialIndex) {
    if (piece->type != EMPTY) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece->rectangle)) {
            piece->isDragged = true;
            state.MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece->type, InitialIndex);

        }
        if (piece->isDragged) {
            piece->rectangle.x = GetMouseX() - piece->rectangle.width / 2;
            piece->rectangle.y = GetMouseY() - piece->rectangle.height / 2;

            //Prevent Pieces from being Dragged out of the Screen
            if (piece->rectangle.x < 0) piece->rectangle.x = 0;
            if (piece->rectangle.y < 0) piece->rectangle.y = 0;
            if (piece->rectangle.x + piece->rectangle.width > GetScreenWidth()) piece->rectangle.x = GetScreenWidth() - piece->rectangle.width;
            if (piece->rectangle.y + piece->rectangle.height > GetScreenHeight()) piece->rectangle.y = GetScreenHeight() - piece->rectangle.height;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                float tileX = round((piece->rectangle.x - BoardOffsetX) / tileSize) * tileSize;
                float tileY = round((piece->rectangle.y - BoardOffsetY) / tileSize) * tileSize;
                int FinalIndex = abs((state.isBoardReversed * (Total_tiles - 1)) - getTileIndex(tileX, tileY, tileSize));
                
                std::string Move = ConvertNotation()(InitialIndex, FinalIndex);

                if (isValidMove(FinalIndex)) {
                    piece->isDragged = false;
                    piece->rectangle.x = tileX;
                    piece->rectangle.y = tileY;

                    MakeCompleteMove(InitialIndex, FinalIndex, Move);
                }
                else {
                    piece->isDragged = false;
                    int file = abs((state.isBoardReversed * ReverseOffset) - (InitialIndex % 8));
                    int rank = abs((state.isBoardReversed * ReverseOffset) - (InitialIndex / 8));
                    piece->rectangle.x = BoardOffsetX + static_cast<float>(file * tileSize);
                    piece->rectangle.y = BoardOffsetY + static_cast<float>(rank * tileSize);
                }
            }
        }
    }
}

bool ChessBoard::isValidMove(int index){
    //Check if the Played Move is Valid
    for (const auto& Move : state.MovesForSelectedPiece) {
        if (index == Move){
            return true;
        }
    }
    return false;
}

void ChessBoard::MakeMove(int fromTile, int toTile) {

    //EnPassant
    if (IsEnPassantCapture(fromTile, toTile)) {
        int captureIndex = (fromTile / 8) * 8 + toTile % 8;
        board[captureIndex] = new ChessPiece();
    }

    std::string move = ConvertNotation()(fromTile, toTile);
    //Castling
    if (IsCastlingMove(move, board[fromTile])) {

        if (toTile == 62 || toTile == 6) { // Kingside castling
            int rookFromTile = (fromTile == 60) ? 63 : 7;
            int rookToTile = (fromTile == 60) ? 61 : 5;
            board[rookFromTile]->firstMove = false;
            board[rookToTile] = board[rookFromTile];
            board[rookFromTile] = new ChessPiece();


        }
        else if (toTile == 58 || toTile == 2) { // Queenside castling
            int rookFromTile = (fromTile == 60) ? 56 : 0;
            int rookToTile = (fromTile == 60) ? 59 : 3;
            board[rookFromTile]->firstMove = false;
            board[rookToTile] = board[rookFromTile];
            board[rookFromTile] = new ChessPiece();
        }
    }

    

    board[fromTile]->firstMove = false;
    board[toTile] = board[fromTile];
    board[fromTile] = new ChessPiece(); // Empty the source square
    state.MovesForSelectedPiece.clear();

    //Reverse board, if Multiplayer
    state.MoveIndices = std::make_pair(fromTile, toTile);
    if (Flags::isMultiplayerGame()) ReverseBoard();
    SetPiecePositions();
}

void ChessBoard::MakeCompleteMove(int fromTile, int toTile, std::string move) {

    //Move not Enpassant so, reset Target
    if (!IsEnPassantCapture(fromTile, toTile) && state.enPassantTarget !=-1) {
        std::cout << "Target Reset " << state.enPassantTarget << std::endl;
        state.enPassantTarget = -1;
    } 

    //Set Enpassant target. There can Only be own Pawn which is an Enpassant target
    if (board[fromTile]->type == PAWN && board[fromTile]->firstMove) {
        if (abs(toTile - fromTile) == 16) {
            int rowFrom = fromTile / 8;
            int rowTo = toTile / 8;

            if ((rowFrom == 1 && rowTo == 3) || (rowFrom == 6 && rowTo == 4)) {
                state.enPassantTarget = (fromTile + toTile) / 2;
                board[state.enPassantTarget]->EnpassantTarget = true;
                //std::cout << "Pawn at " << toTile << " is an Enpassant target" << " " << enPassantTarget << std::endl;
            }
        }
    }


    state.PieceIsCaptured = isValidCaptureMove(fromTile, toTile);
    MakeMove(fromTile, toTile);

    // Pawn promotion
    if (board[toTile]->type == PAWN) {
        // White pawn promotion
        if (toTile >= 0 && toTile <= 7) {
            promotePawn(toTile);
        }
        // Black pawn promotion
        else if (toTile >= 56 && toTile <= 63) {
            promotePawn(toTile);
        }
    }

    std::cout << (state.currentPlayerIsWhite ? "White" : "Black") << " Plays : " << move << "\n";

    if (state.moveHistory.size() >= MaxHistorySize) {
        state.moveHistory.clear();
    }

    AddMoveToHistory(move);
    state.MoveIndices = std::make_pair(fromTile, toTile);
    SetPiecePositions();

    //Uncomment for Debugging or to Show Board on terminal
    //DebugItem()(board);
    //DisplayScores();

    isCheck(*this, state.currentPlayerIsWhite ? Black : White, "ChessBoard::MakeCompleteMove");


    Flags::CheckGameState();   
    state.currentPlayerIsWhite = !state.currentPlayerIsWhite;
    ComputeOpponentMoves();
    PlayChessSound();
    state.MovesForSelectedPiece.clear();
}


void ChessBoard::ReverseBoard() {
    state.isBoardReversed = !state.isBoardReversed;
}

bool ChessBoard::IsCastlingMove(std::string move, ChessPiece* pieceMoved) {
    return (pieceMoved->type == KING) && (move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8");
}

bool ChessBoard::IsEnPassantCapture(int fromTile, int toTile) const {
    ChessPiece* piece = board[fromTile];

    if (piece->type == PAWN && toTile == state.enPassantTarget) {
        if ((piece->color == White && fromTile/8 == 3) || (piece->color == Black && fromTile/8 == 4)) {
            return true;
        }
    }
    return false;
}

void ChessBoard::PlayChessSound() const {
    if (state.checkedPlayer != 0) {
        PlaySound(KingChecked);
    }
    else if (state.PieceIsCaptured) {
        PlaySound(ChessPieceCaptured);
    }
    else {
        PlaySound(ChessPiecePlaced);
    }
}

bool ChessBoard::isCurrentPlayerWhite() const{
    return state.currentPlayerIsWhite;
}

void ChessBoard::DisplayScores() const {
    int whiteScore = calculatePlayerScore(White);
    int blackScore = calculatePlayerScore(Black);
    std::cout << "\nWhite score: " << whiteScore << " | Black score: " << blackScore << "\n\n";
}

void ChessBoard::ComputeSlidingPieceMoves(int pieceIndex, std::vector<int>& possibleMoves) const {
    ChessPiece* piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Offsets for sliding directions: up, down, left, right, diagonals
    std::vector<int> offsets = { 8, -8, 1, -1, 7, -7, 9, -9 };

    if (piece->type == ROOK || piece->type == QUEEN) {
        for (int i = 0; i < 4; ++i) {
            int offset = offsets[i];
            int newIndex = pieceIndex + offset;
            while (newIndex >= 0 && newIndex < Total_tiles) {
                // Check if the new index is within the bounds of the board
                if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                    break; // If moving horizontally, ensure it stays on the same row
                if (board[newIndex]->type != EMPTY) {
                    if (board[newIndex]->color != piece->color) {
                        // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                        possibleMoves.push_back(newIndex);
                    }
                    break; // Stop searching in this direction if we encounter a piece
                }
                possibleMoves.push_back(newIndex);
                newIndex += offset;
            }
        }
    }

    if (piece->type == BISHOP || piece->type == QUEEN) {
        for (int i = 4; i < 8; ++i) {
            int offset = offsets[i];
            int newIndex = pieceIndex + offset;
            while (newIndex >= 0 && newIndex < Total_tiles) {
                // Check if the new index is within the bounds of the board
                int newRow = newIndex / 8;
                int newCol = newIndex % 8;
                if (abs(newRow - row) != abs(newCol - col))
                    break; // If not moving diagonally, break
                if (board[newIndex]->type != EMPTY) {
                    if (board[newIndex]->color != piece->color) {
                        // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                        possibleMoves.push_back(newIndex);
                    }
                    break; // Stop searching in this direction if we encounter a piece
                }
                possibleMoves.push_back(newIndex);
                newIndex += offset;
            }
        }
    }
}

void ChessBoard::ComputeKnightMoves(int pieceIndex, std::vector<int>& possibleMoves) const {
    ChessPiece* piece = board[pieceIndex];
    // Offsets for knight's moves
    std::vector<int> rowOffsets = { -2, -1, 1, 2, 2, 1, -1, -2 };
    std::vector<int> colOffsets = { 1, 2, 2, 1, -1, -2, -2, -1 };

    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    if (piece->type == KNIGHT) {
        for (int i = 0; i < 8; ++i) {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if (board[newIndex]->type == EMPTY || board[newIndex]->color != piece->color) {
                    possibleMoves.push_back(newIndex);
                }
            }
        }
    }
}

void ChessBoard::ComputePawnMoves(int pieceIndex, std::vector<int>& possibleMoves) const {
    ChessPiece* piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Determine the direction of pawn movement based on its color
    int direction = (piece->color == White) ? -1 : 1;

    // Forward movement
    int forwardOne = pieceIndex + direction * 8;
    if (forwardOne >= 0 && forwardOne < Total_tiles && board[forwardOne]->type == EMPTY) {
        possibleMoves.push_back(forwardOne);
        // Check if the pawn is in its starting position and can move two squares forward
        if ((row == 6 && piece->color == White) || (row == 1 && piece->color == Black)) {
            int forwardTwo = forwardOne + direction * 8;
            if (board[forwardTwo]->type == EMPTY) {
                possibleMoves.push_back(forwardTwo);
            }
        }
    }

    // Capture moves and en passant
    std::vector<int> captureOffsets = { direction * 7, direction * 9 };
    for (int offset : captureOffsets) {
        int targetIndex = pieceIndex + offset;
        int targetRow = targetIndex / 8;
        int targetCol = targetIndex % 8;
        int pieceCol = pieceIndex % 8;
        // Ensure target column is within bounds and not wrapping around the board
        if (targetCol >= 0 && targetCol < 8 && abs(targetCol - pieceCol) <= 1) {
            // Check if the target square is occupied by an opponent's piece
            if (board[targetIndex]->type != EMPTY && board[targetIndex]->color != piece->color) {
                possibleMoves.push_back(targetIndex);
            }
            // Check for en passant
            if (state.enPassantTarget != -1) {
                // Check if the target index is the en passant target
                if (targetIndex == state.enPassantTarget) {
                    // Check if the current piece can perform en passant
                    if ((piece->color == White && row == 3) || (piece->color == Black && row == 4)) {
                        possibleMoves.push_back(targetIndex);
                    }
                }
            }
        }
    }
}

void ChessBoard::ComputeKingMoves(int KingIndex, std::vector<int>& possibleMoves) const {

    ChessPiece* King = board[KingIndex];
    int row = KingIndex / 8;
    int col = KingIndex % 8;

    // Offsets for king's moves
    std::vector<int> rowOffsets = { -1, -1, -1, 0, 0, 1, 1, 1 };
    std::vector<int> colOffsets = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // Check all 8 directions
    for (int i = 0; i < 8; i++) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            int newIndex = newRow * 8 + newCol;
            // Check if the square is empty or contains an opponent's piece
            if ((board[newIndex]->type == EMPTY || board[newIndex]->color != King->color)) {
                possibleMoves.push_back(newIndex);
            }
        }
    }

    if ((King->color == Black && KingIndex == 4) || (King->color == White && KingIndex == 60)) {

        // Check for castling moves
        if (King->firstMove && !IsTileUnderAttack(KingIndex)) {
            // Check kingside castling
            if (canCastleKingSide(KingIndex)) {

                possibleMoves.push_back(KingIndex + 2);
            }
            // Check queenside castling
            if (canCastleQueenide(KingIndex)) {
                
                possibleMoves.push_back(KingIndex - 2);
            }
        }
    }
}

bool ChessBoard::IsEnPassantLegal(int pawnIndex, int targetIndex) const {
    ChessPiece* pawn = board[pawnIndex];
    int row = pawnIndex / 8;
    int col = pawnIndex % 8;
    int targetRow = targetIndex / 8;
    int targetCol = targetIndex % 8;

    // Check if the target square is empty
    if (board[targetIndex]->type == EMPTY) {
        // Check if the target square is on the same row as the pawn
        if (row == targetRow) {
            // Check if the pawn is in the correct starting position to perform en passant
            if ((pawn->color == White && row == 3) || (pawn->color == Black && row == 4)) {
                // Check if there's an opponent's pawn on the adjacent column
                if (abs(col - targetCol) == 1 && board[targetIndex - 8]->type == PAWN && board[targetIndex - 8]->color != pawn->color) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessBoard::canCastleKingSide(int KingIndex) const {
    // ChessPiece.canCastleKingSide is a boolean flag that is set true when Initialising the board
    if (!board[KingIndex]->canCastleKingSide) return false;

    int RookIndex = KingIndex + 3;
    if (board[RookIndex]->type != ROOK && !(board[RookIndex]->firstMove)) return false;

    for (int i = KingIndex + 1; i < RookIndex; i++) {
        if (board[i]->type != EMPTY || IsTileUnderAttack(i)) return false;
    }

    return true;
}

bool ChessBoard::canCastleQueenide(int KingIndex) const {
    // ChessPiece.canCastleQueenSide is a boolean flag that is set true when Initialising the board
    if (!board[KingIndex]->canCastleQueenSide)return false;
   
    int RookIndex = KingIndex - 4;
    if (board[RookIndex]->type != ROOK && !(board[RookIndex]->firstMove)) return false;

    for (int i = KingIndex - 1; i > RookIndex; i--) {
        if (board[i]->type != EMPTY || IsTileUnderAttack(i)) {
            return false;
        }
    }

    return true;
}

bool ChessBoard::isCheck(const ChessBoard& chessboard, const int playerColor, std::string calledby) {
    // The std::string CalledBy is important for Debugging Reasons
    // I found that most of the errors/bugs orignated from this function, and by who or how it was being called
    //chessboard.DisplayBoard();

    state.checkedPlayer = 0; // No Player is Checked
    int kingIndex = -100;
    for (int i = 0; i < Total_tiles; i++) {
        if (chessboard.board[i]->type == KING && chessboard.board[i]->color == playerColor) {
            kingIndex = i;
            break;
        }
    }

    //Exit the Game, If Kind is not found, and show Who called this function
    if (kingIndex == -100) {
        std::cout << "\nError Code 404: King not found-> \n\n";
        std::cout << calledby << "\n\n";
        exit(404);
        return false;
    }

    // Iterate through opponent's pieces and see if any can attack the king
    int opponentColor = (playerColor == White) ? Black : White;
    std::vector<int> possibleMoves;
    for (int i = 0; i < Total_tiles; i++) {
        if (chessboard.board[i]->color == opponentColor) {
            if (chessboard.board[i]->isSlidingPiece) ComputeSlidingPieceMoves(i, possibleMoves);
            else if (chessboard.board[i]->type == PAWN) ComputePawnMoves(i, possibleMoves);
            else if (chessboard.board[i]->type == KNIGHT) ComputeKnightMoves(i, possibleMoves);
            else if (chessboard.board[i]->type == KING) ComputeKingMoves(i, possibleMoves);

            // Check if any of the possible moves include the king's position
            for (int move : possibleMoves) {
                if (move == kingIndex) {
                    state.checkedPlayer = playerColor;
                    
                    return true; // King is in check
                }
            }
        }
    }

    return false; // King is not in check
}

bool ChessBoard::isCheckmate(ChessBoard& chessboard, const int playerColor) const {
    // King is not in check, so it can't be checkmate
    if (state.checkedPlayer == 0) return false; 
   
    std::vector<int> possibleMoves = GetAllPossibleMoves(playerColor);
    if (possibleMoves.size() > 0) return false; // King can escape from check, so it's not checkmate

    return true; // King is in checkmate
}

std::vector<int> ChessBoard::GetAllPossibleMoves(int playerColor) const {

    std::vector<int> possibleMoves;

    for (int i = 0; i < Total_tiles; i++) {
        if (board[i]->color == playerColor) {
            std::vector<int> moves = GetAllPossibleMovesForPiece(board[i]->type, i);

            moves = FilterValidMoves(i, moves);
            for (auto move : moves) {
                possibleMoves.push_back(move);
            }
        }
    }

    return possibleMoves;
}

std::vector<std::string> ChessBoard::GetAllPossibleMovesInChessNotation(int playerColor) const {
    std::vector<std::string> possibleMoves;

    for (int i = 0; i < Total_tiles; i++) {
        if (board[i]->color == playerColor) {
            std::vector<int> moves = GetAllPossibleMovesForPiece(board[i]->type, i);

            // Check if the move is legal (does not leave the king in check)
            for (int toTile : moves) {
                // Simulate the move
                ChessBoard temp(*this);
                temp.MakeMove(i, toTile);

                // Check if the king is in check after the move
                if (!temp.isCheck(temp, playerColor, "board: Get all possible moves in chess notation")) {
                    // If not in check, add the move to possible moves after converting to ChessNotation
                    possibleMoves.push_back(ConvertNotation()(i, toTile));
                }
            }
        }
    }
    return possibleMoves;
}
int ChessBoard::getAttacksOnSquare(int squareIndex, int opponentColor) const {
    int attacks = 0;
    std::vector<int> opponentMoves = GetAllPossibleMoves(opponentColor);
    for (int i = 0; i < opponentMoves.size(); ++i) {
        if (opponentMoves[i] == squareIndex) {
            attacks++;
        }
    }
    return attacks;
}

bool ChessBoard::IsTileUnderAttack(int squareIndex) const {

    for (int i = 0; i < state.OpponentMoves.size(); i++) {
        if (state.OpponentMoves[i] == squareIndex) {
            return true;
        }
    }
    return false;
}

void ChessBoard::DisplayMoves() {
    for (auto move : state.OpponentMoves) {
        std::cout << move << std::endl;
    }
}

std::vector<int> ChessBoard::GetAllPossibleMovesForPiece(int type, int index) const {
    std::vector<int> moves;

    // Compute moves based on the piece type
    if (type == ROOK || type == BISHOP || type == QUEEN) {
        ComputeSlidingPieceMoves(index, moves);
    }
    else if (type == PAWN) {
        ComputePawnMoves(index, moves);
    }
    else if (type == KNIGHT) {
        ComputeKnightMoves(index, moves);
    }
    else if (type == KING) {
        ComputeKingMoves(index, moves);
    }

    moves = FilterValidMoves(index, moves);
    return moves;
}

void ChessBoard::DestroyBoard() {
    state.reset();
    for (int i = 0; i < Total_tiles; i++) {
        board[i]->DestroyTextures();
    }
}

ChessPiece* ChessBoard::GetPieceAtPosition(int index) const {
    return board[index];
}

std::vector<std::string> ChessBoard::GetAllCaptureMovesInChessNotation(int color) const {
    std::vector<std::string> captureMoves;

    for (int fromTile = 0; fromTile < Total_tiles; fromTile++) {
        ChessPiece* piece = board[fromTile];
        if (piece->color == color) {
            for (int toTile = 0; toTile < Total_tiles; toTile++) {
                if (fromTile != toTile && isValidCaptureMove(fromTile, toTile)) {
                    std::string move = ConvertNotation()(fromTile, toTile);
                    captureMoves.push_back(move);
                }
            }
        }
    }

    return captureMoves;
}

bool ChessBoard::isValidCaptureMove(int fromTile, int toTile) const {
    ChessPiece* fromPiece = board[fromTile];
    ChessPiece* toPiece = board[toTile];
    if (IsEnPassantCapture(fromTile, toTile)) {
        return true;
    }

    // Check if there is a piece at the 'from' tile and it is of opposite color to the piece at the 'to' tile
    return fromPiece->type != EMPTY && toPiece->type != EMPTY && fromPiece->color != toPiece->color;
}

void ChessBoard::ComputeOpponentMoves() {
    state.OpponentMoves = GetAllPossibleMoves(state.currentPlayerIsWhite ? Black : White);
}

void ChessBoard::LoadTextures(){
    for (int index = 0; index < Total_tiles; index++) {
        board[index]->AssignTextures();
    }
}

