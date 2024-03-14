#include "ChessBoard.h"



ChessBoard::ChessBoard() {
    currentPlayerIsWhite = true;
    //for (ChessPiece chesspiece : board) {
    //    chesspiece = ChessPiece(EMPTY, EMPTY);
    //}
    DoCastle = false;
    MoveIndices = { -1, -1 };
    isBoardReversed = false;
    PieceIsCaptured = false;
}

int ChessBoard::getPieceScore(const int& type) const {
    switch (type) {
    case PAWN: return pawnScore;
    case ROOK: return rookScore;
    case KNIGHT: return knightScore;
    case BISHOP: return bishopScore;
    case QUEEN: return queenScore;
    case KING: return kingScore;
    default: return 0; // Unknown piece type
    }
}

void ChessBoard::AddMoveToHistory(string move) {
    moveHistory.push_back(move);
}

void ChessBoard::promotePawn(int toTile) {
    bool PawnPromoted = false;
    // cout << "\nPawn promotion! Choose a piece (Q, R, N, B): ";

    //ChessPiece promotedPiece;
    //
    // if (IsKeyPressed(KEY_Q)) {
    //    promotedPiece = ChessPiece(currentPlayerIsWhite ? QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Queen" << endl;
    //    PawnPromoted = false;
    //    //break;
    //}
    // else if (IsKeyPressed(KEY_R)) {
    //    promotedPiece = ChessPiece(currentPlayerIsWhite ? ROOK) : tolower(ROOK), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Rook" << endl;
    //    //break;
    //}
    // else if (IsKeyPressed(KEY_N)) {
    //    promotedPiece = ChessPiece(currentPlayerIsWhite ? KNIGHT) : tolower(KNIGHT), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Knight" << endl;
    //    //break;
    //}
    // else if (IsKeyPressed(KEY_B)) {
    //    promotedPiece = ChessPiece(currentPlayerIsWhite ? BISHOP) : tolower(BISHOP), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Bishop" << endl;
    //    //break;
    //}
    // else {
    //    cout << "Invalid choice. Defaulting to Queen promotion." << endl;
    //    promotedPiece = ChessPiece(currentPlayerIsWhite ? QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
    //    cout << "Piece Promoted to Queen" << endl;
    //    //break;
    //}

    ChessPiece promotedPiece = ChessPiece(QUEEN, currentPlayerIsWhite ? White : Black);
    // cout << "Piece Promoted to Queen" << endl;
    // board[toTile].DestroyTextures();
    //  Update the board with the promoted piece
    board[toTile] = promotedPiece;
    SetPiecePositions();
}

vector<int> ChessBoard::FilterValidMoves(int fromIndex, vector<int> possibleMoves) const {
    ChessPiece piece = board[fromIndex];

    // Filter out moves that leave the king in check
    vector<int> filteredMoves;
    for (int toTile : possibleMoves) {
        // Simulate the move on a temporary board
        ChessBoard tempBoard345 = *this;
        tempBoard345.MakeMove(fromIndex, toTile);

        // Check if the king is in check after the move
        if (!tempBoard345.isCheck(tempBoard345, piece.color, "Board: Filter Valid Moves")) {
            filteredMoves.push_back(toTile);
        }
    }
    return filteredMoves;
}

int ChessBoard::calculatePlayerScore(int playerColor) const {
    int totalScore = 0;
    for (int i = 0; i < Total_tiles; i++) {
        if (board[i].color == playerColor) {
            totalScore += getPieceScore(board[i].type);
        }
    }
    return totalScore;
}

void ChessBoard::initializeBoardFromFEN(const string& fen) {
    moveHistory.clear();
    MoveIndices = { -1,-1 };
    int row = 0, col = 0;
    // Parse the FEN string
    for (char symbol : fen) {
        if (symbol == ' ') {
            break;  // Stop parsing after the board section
        }
        if (symbol == '/') {
            row++;
            col = 0;
        }
        else if (isdigit(symbol)) {
            // Fill empty squares
            int numEmptySquares = symbol - '0';
            for (int i = 0; i < numEmptySquares; i++) {
                board[row * 8 + col] = ChessPiece(EMPTY, EMPTY);
                col++;
            }
        }
        else {
            // Set piece based on FEN character
            int pieceType = EMPTY;
            int pieceColor = (isupper(symbol) ? White : Black);
            switch (toupper(symbol)) {
            case 'K': pieceType = KING; break;
            case 'P': pieceType = PAWN; break;
            case 'N': pieceType = KNIGHT; break;
            case 'B': pieceType = BISHOP; break;
            case 'R': pieceType = ROOK; break;
            case 'Q': pieceType = QUEEN; break;
            default: break;
            }
            board[row * 8 + col] = ChessPiece(pieceType, pieceColor);
            col++;
        }
    }
    // Set the current player based on FEN
    currentPlayerIsWhite = (fen.find("w") != string::npos);
    int WhiteKingIndex = GetKingIndex(White);
    int BlackKingIndex = GetKingIndex(Black);
    size_t castlingRightsPos = fen.find_last_of(" ");
    if (castlingRightsPos != string::npos && castlingRightsPos + 1 < fen.length()) {
        string castlingRights = fen.substr(castlingRightsPos + 1);
        for (char c : castlingRights) {
            if (c == 'K') {
                board[WhiteKingIndex].canCastleKingSide = true;
            }
            else if (c == 'Q') {
                board[WhiteKingIndex].canCastleQueenSide = true;
            }
            else if (c == 'k') {
                board[BlackKingIndex].canCastleKingSide = true;
            }
            else if (c == 'q') {
                board[BlackKingIndex].canCastleQueenSide = true;
            }
        }
    }

    // Set piece positions and additional FEN features if needed
    SetPiecePositions();
    // ...code for additional FEN features
}

int ChessBoard::GetKingIndex(const int& playercolor) const {
    for (int index = 0; index < Total_tiles; index++) {
        if (board[index].type == KING && board[index].color == playercolor) {
            return index;
        }
    }

    cout << "\n\nGetKingIndex: KING not Found\n\n";
    exit(404);

}

// Function to generate and save FEN string to file
void ChessBoard::saveMatchHistoryToFile() const {
    // Open file for writing (in append mode)
    ofstream file("MatchHistory.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    // Generate FEN string from current board state
    string fen;
    for (int row = 0; row < 8; row++) {
        int emptyCount = 0;
        for (int col = 0; col < 8; col++) {
            int index = row * 8 + col;
            if (board[index].type == EMPTY) {
                emptyCount++;
            }
            else {
                if (emptyCount > 0) {
                    fen += to_string(emptyCount);
                    emptyCount = 0;
                }
                switch (board[index].type) {
                case KING:
                    fen += (board[index].color == White) ? 'K' : 'k';
                    break;
                case PAWN:
                    fen += (board[index].color == White) ? 'P' : 'p';
                    break;
                case KNIGHT:
                    fen += (board[index].color == White) ? 'N' : 'n';
                    break;
                case BISHOP:
                    fen += (board[index].color == White) ? 'B' : 'b';
                    break;
                case ROOK:
                    fen += (board[index].color == White) ? 'R' : 'r';
                    break;
                case QUEEN:
                    fen += (board[index].color == White) ? 'Q' : 'q';
                    break;
                default:
                    // Handle unexpected piece type
                    cerr << "Error: Unexpected piece type encountered." << endl;
                    break;
                }
            }
        }
        if (emptyCount > 0) {
            fen += to_string(emptyCount);
        }
        if (row < 7) {
            fen += '/';
        }
    }
    fen += (currentPlayerIsWhite ? " w " : " b ");
    // Add additional FEN features if needed

    // Write FEN string to file
    file << fen << endl; // Append newline after each FEN string
    // Close file
    file.close();

    cout << "Saved FEN string" << endl;
}

void ChessBoard::InitializeDefaultBoard() {

    currentPlayerIsWhite = true;
    // Set up the board with pieces in their initial positions
    string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    initializeBoardFromFEN(DefaultFEN);
}

void ChessBoard::initializeBoard() {
    // Set up the board with pieces in their initial positions

    // Black pieces
    board[0] = ChessPiece(ROOK, Black);
    board[1] = ChessPiece(KNIGHT, Black);
    board[2] = ChessPiece(BISHOP, Black);
    board[3] = ChessPiece(QUEEN, Black);
    board[4] = ChessPiece(KING, Black);
    board[5] = ChessPiece(BISHOP, Black);
    board[6] = ChessPiece(KNIGHT, Black);
    board[7] = ChessPiece(ROOK, Black);
    for (int i = 8; i <= 15; i++) {
        board[i] = ChessPiece(PAWN, Black);
    }

    // Empty spaces in the middle
    for (int i = 16; i <= 47; i++) {
        board[i] = ChessPiece(EMPTY, EMPTY);

    }

    // White pieces
    for (int i = 48; i <= 55; i++) {
        board[i] = ChessPiece(PAWN, White);
    }
    board[56] = ChessPiece(ROOK, White);
    board[57] = ChessPiece(KNIGHT, White);
    board[58] = ChessPiece(BISHOP, White);
    board[59] = ChessPiece(QUEEN, White);
    board[60] = ChessPiece(KING, White);
    board[61] = ChessPiece(BISHOP, White);
    board[62] = ChessPiece(KNIGHT, White);
    board[63] = ChessPiece(ROOK, White);
    SetPiecePositions();
}

void ChessBoard::DisplayBoard() const {
    cout << "8 ";
    for (int i = 0; i < Total_tiles; i++) {
        // cout << (i/8) + 1  << " ";
        cout << "[" << board[i].type << "]";
        if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles) {
            cout << endl;
            cout << 8 - (i + 1) / 8 << " ";
        }

        // cout << endl;
    }
    cout << endl;
    cout << "   a  b  c  d  e  f  g  h" << endl;
}

pair<int, int> ChessBoard::PieceCoordinates(int pieceIndex) const {
    if (pieceIndex < 0 || pieceIndex >= Total_tiles) {
        return make_pair(-1, -1);
    }
    int rank = pieceIndex / boardSize;
    int file = pieceIndex % boardSize;
    return make_pair(rank, file);
}

void ChessBoard::SetPiecePositions() {
    // Set initial positions of the pieces
    for (int index = 0; index < Total_tiles; index++) {
        pair<int, int> PieceCoords = PieceCoordinates(index);
        board[index].rectangle.x = BoardOffsetX + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize);
        board[index].rectangle.y = BoardOffsetY + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize);
    }
}

void ChessBoard::DrawBoard() const {
    for (int index = 0; index < Total_tiles; index++) {

        pair<int, int> PieceCoords = PieceCoordinates(index);

        Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

        int LocationX = BoardOffsetX + (abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocationY = BoardOffsetY + (abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);

        // if (!FinalIndexes.isEmpty() && FinalIndexes.peek() == index) {
        //     DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

        //}
        // else if (FinalIndexes.CompareIndex(index) && FinalIndexes.peek() != index) {
        //    DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

        //}

        // Highlight Last Move
        if (MoveIndices.first == index || MoveIndices.second == index) {
            DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
        }
        // Show Moves for Piece
        for (const auto& move : MovesForSelectedPiece) {
            if (move == index) {
                float Diameter = 26;
                float Radius = Diameter / 2;
                float Offset = Diameter + Radius;

                Rectangle CorrectMoves = { static_cast<float>(LocationX) + Offset, static_cast<float>(LocationY) + Offset, Diameter, Diameter };
                DrawCircle(LocationX + static_cast<int>(Offset), LocationY + static_cast<int>(Offset), Radius, Translucent);
            }
        }
    }
}

void ChessBoard::DrawChessPiece() const {
    for (int index = 0; index < Total_tiles; index++) {

        int PositionX = static_cast<int>(board[index].rectangle.x);
        int PositionY = static_cast<int>(board[index].rectangle.y);
        DrawTexture(board[index].texture, PositionX, PositionY, WHITE);
    }
}

int ChessBoard::getTileIndex(float x, float y, int tileSize) {

    int file = static_cast<int>(x / tileSize);
    int rank = static_cast<int>(y / tileSize);
    int index = rank * 8 + file;

    return index;
}

void ChessBoard::UpdateChessPiece(ChessPiece& piece, int InitialIndex) {
    if (piece.type != EMPTY) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rectangle)) {
            piece.isDragged = true;
            MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece.type, InitialIndex);
            // DisplayMoves(MovesForSelectedPiece);
            // cout << InitialIndex << endl;
        }
        if (piece.isDragged) {
            piece.rectangle.x = GetMouseX() - piece.rectangle.width / 2;
            piece.rectangle.y = GetMouseY() - piece.rectangle.height / 2;

            //Prevent Pieces from being Dragged out of the Screen
            if (piece.rectangle.x < 0) piece.rectangle.x = 0;
            if (piece.rectangle.y < 0) piece.rectangle.y = 0;
            if (piece.rectangle.x + piece.rectangle.width > GetScreenWidth()) piece.rectangle.x = GetScreenWidth() - piece.rectangle.width;
            if (piece.rectangle.y + piece.rectangle.height > GetScreenHeight()) piece.rectangle.y = GetScreenHeight() - piece.rectangle.height;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                float tileX = round((piece.rectangle.x - BoardOffsetX) / tileSize) * tileSize;
                float tileY = round((piece.rectangle.y - BoardOffsetY) / tileSize) * tileSize;
                int FinalIndex = abs((isBoardReversed * (Total_tiles - 1)) - getTileIndex(tileX, tileY, tileSize));
                //cout << isBoardReversed * ReverseOffset << " " << FinalIndex << endl;
                string Move = ConvertToChessNotation(InitialIndex, FinalIndex);

                //cout << Move << endl;

                // cout << tileX << " " << tileY << endl;
                if (isValidMove(FinalIndex)) {
                    piece.isDragged = false;
                    piece.rectangle.x = tileX;
                    piece.rectangle.y = tileY;

                    MakeCompleteMove(InitialIndex, FinalIndex, Move);
                    //DisplayBoard();
                    // WaitTime(0.5);
                }
                else {
                    piece.isDragged = false;
                    int file = abs((isBoardReversed * ReverseOffset) - (InitialIndex % 8));
                    int rank = abs((isBoardReversed * ReverseOffset) - (InitialIndex / 8));
                    piece.rectangle.x = BoardOffsetX + static_cast<float>(file * tileSize);
                    piece.rectangle.y = BoardOffsetY + static_cast<float>(rank * tileSize);
                }
            }
        }
    }
}

bool ChessBoard::isValidMove(int index)
{
    for (auto Move : MovesForSelectedPiece)
    {
        if (index == Move)
        {
            return true;
        }
    }
    return false;
}

// void MakeMove(int fromTile, int toTile)
//{
//     string move = ConvertToChessNotation(fromTile, toTile);
//     // Check if it's a castling move and if castling is allowed
//     if (IsCastlingMove(move, board[fromTile]))
//     {
//         if (toTile == 62 || toTile == 7)
//         { // Kingside castling
//             int rookFromTile = (fromTile == 60) ? 63 : 7;
//             int rookToTile = (fromTile == 60) ? 61 : 5;
//             board[rookToTile] = board[rookFromTile];
//             board[rookFromTile] = ChessPiece();
//         }
//         else if (toTile == 58 || toTile == 2)
//         { // Queenside castling
//             int rookFromTile = (fromTile == 60) ? 56 : 0;
//             int rookToTile = (fromTile == 60) ? 59 : 3;
//             board[rookToTile] = board[rookFromTile]; // Move the rook
//             board[rookFromTile] = ChessPiece();      // Empty the original rook square
//         }
//     }
//     board[fromTile].firstMove = false;
//     board[toTile] = board[fromTile];
//     board[fromTile] = ChessPiece(); // Empty the source square
// }
// bool IsCastlingMove(string move, ChessPiece pieceMoved)
//{
//     return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || "e8g8" || move == "e8c8");
// }

void ChessBoard::MakeMove(int fromTile, int toTile) {
    string move = ConvertToChessNotation(fromTile, toTile);

    // Check if it's a castling move and if castling is allowed
    if (IsCastlingMove(move, board[fromTile])) {

        if (toTile == 62 || toTile == 6) { // Kingside castling
            int rookFromTile = (fromTile == 60) ? 63 : 7;
            int rookToTile = (fromTile == 60) ? 61 : 5;
            board[rookFromTile].firstMove = false;
            board[rookToTile] = board[rookFromTile];
            board[rookFromTile] = ChessPiece(0, 0);


        }
        else if (toTile == 58 || toTile == 2) { // Queenside castling
            int rookFromTile = (fromTile == 60) ? 56 : 0;
            int rookToTile = (fromTile == 60) ? 59 : 3;
            board[rookFromTile].firstMove = false;
            board[rookToTile] = board[rookFromTile];
            board[rookFromTile] = ChessPiece();
        }
    }

    board[fromTile].firstMove = false;
    board[toTile] = board[fromTile];
    board[fromTile] = ChessPiece(); // Empty the source square
    MovesForSelectedPiece.clear();

    if (flags.isMultiplayerGame()) ReverseBoard();
}

void ChessBoard::ReverseBoard() {
    isBoardReversed = !isBoardReversed;

}

bool ChessBoard::IsCastlingMove(string move, ChessPiece pieceMoved) {

    return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8");
}

void ChessBoard::MakeCompleteMove(int fromTile, int toTile, string move) {
    // string move = ConvertToChessNotation(Move);
    PieceIsCaptured = isValidCaptureMove(fromTile, toTile);
    MakeMove(fromTile, toTile);

    // Check for pawn promotion
    if (board[toTile].type == PAWN) {
        // White pawn promotion
        if (toTile >= 0 && toTile <= 7) {
            promotePawn(toTile);
        }
        // Black pawn promotion
        else if (toTile >= 56 && toTile <= 63) {
            cout << "Test";
            promotePawn(toTile);
        }
    }

    cout << (currentPlayerIsWhite ? "White" : "Black") << " Plays : " << move << "\n";

    if (moveHistory.size() >= MaxHistorySize) {
        moveHistory.clear();
    }


    flags.CheckGameState();
    AddMoveToHistory(move);
    MoveIndices = make_pair(fromTile, toTile);

    SetPiecePositions();
    //DisplayScores();
    currentPlayerIsWhite = !currentPlayerIsWhite;
    ComputeOpponentMoves();
    PlayChessSound();
    MovesForSelectedPiece.clear();
}

void ChessBoard::PlayChessSound() const {
    ChessBoard temp = *this;
    int PlayerColor = (temp.isCurrentPlayerWhite()) ? White : Black;
    if (isCheck(temp, PlayerColor, "board: Play Chess Sound")) {
        PlaySound(KingChecked);
    }
    else if (PieceIsCaptured) {
        PlaySound(ChessPieceCaptured);
    }
    else {
        PlaySound(ChessPiecePlaced);
    }
}

bool ChessBoard::isCurrentPlayerWhite() const
{
    return currentPlayerIsWhite;
}

void ChessBoard::DisplayScores() const {
    int whiteScore = calculatePlayerScore(White);
    int blackScore = calculatePlayerScore(Black);
    cout << "\nWhite score: " << whiteScore << " | Black score: " << blackScore << "\n\n";
}

void ChessBoard::ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const {
    ChessPiece piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Offsets for sliding directions: up, down, left, right, diagonals
    vector<int> offsets = { 8, -8, 1, -1, 7, -7, 9, -9 };

    if (piece.type == ROOK || piece.type == QUEEN) {
        for (int i = 0; i < 4; ++i) {
            int offset = offsets[i];
            int newIndex = pieceIndex + offset;
            while (newIndex >= 0 && newIndex < Total_tiles) {
                // Check if the new index is within the bounds of the board
                if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                    break; // If moving horizontally, ensure it stays on the same row
                if (board[newIndex].type != EMPTY) {
                    if (board[newIndex].color != piece.color) {
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

    if (piece.type == BISHOP || piece.type == QUEEN) {
        for (int i = 4; i < 8; ++i) {
            int offset = offsets[i];
            int newIndex = pieceIndex + offset;
            while (newIndex >= 0 && newIndex < Total_tiles) {
                // Check if the new index is within the bounds of the board
                int newRow = newIndex / 8;
                int newCol = newIndex % 8;
                if (abs(newRow - row) != abs(newCol - col))
                    break; // If not moving diagonally, break
                if (board[newIndex].type != EMPTY) {
                    if (board[newIndex].color != piece.color) {
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

void ChessBoard::ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const {
    ChessPiece piece = board[pieceIndex];
    // Offsets for knight's moves
    vector<int> rowOffsets = { -2, -1, 1, 2, 2, 1, -1, -2 };
    vector<int> colOffsets = { 1, 2, 2, 1, -1, -2, -2, -1 };

    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    if (piece.type == KNIGHT) {
        for (int i = 0; i < 8; ++i) {
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if (board[newIndex].type == EMPTY || board[newIndex].color != piece.color) {
                    possibleMoves.push_back(newIndex);
                }
            }
        }
    }
}

void ChessBoard::ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const {
    ChessPiece piece = board[pieceIndex];
    int row = pieceIndex / 8;
    int col = pieceIndex % 8;

    // Determine the direction of pawn movement based on its color
    int direction = (piece.color == White) ? -1 : 1;

    // Forward movement
    int forwardOne = pieceIndex + direction * 8;
    if (forwardOne >= 0 && forwardOne < Total_tiles && board[forwardOne].type == EMPTY) {
        possibleMoves.push_back(forwardOne);
        // Check if the pawn is in its starting position and can move two squares forward
        if ((row == 6 && piece.color == White) || (row == 1 && piece.color == Black)) {
            int forwardTwo = forwardOne + direction * 8;
            if (board[forwardTwo].type == EMPTY) {
                possibleMoves.push_back(forwardTwo);
            }
        }
    }

    // Capture moves
    vector<int> captureOffsets = { direction * 7, direction * 9 };
    for (int offset : captureOffsets) {
        int targetIndex = pieceIndex + offset;
        int targetRow = targetIndex / 8;
        int targetCol = targetIndex % 8;
        int pieceCol = pieceIndex % 8;
        // Ensure target column is within bounds and not wrapping around the board
        if (targetCol >= 0 && targetCol < 8 && abs(targetCol - pieceCol) <= 1) {
            // Check if the target square is occupied by an opponent's piece
            if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color) {
                possibleMoves.push_back(targetIndex);
            }
        }
    }
}

void ChessBoard::ComputeKingMoves(int KingIndex, vector<int>& possibleMoves) const {

    ChessPiece King = board[KingIndex];
    int row = KingIndex / 8;
    int col = KingIndex % 8;

    // Offsets for king's moves
    vector<int> rowOffsets = { -1, -1, -1, 0, 0, 1, 1, 1 };
    vector<int> colOffsets = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // Check all 8 directions
    for (int i = 0; i < 8; i++) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];

        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            int newIndex = newRow * 8 + newCol;
            // Check if the square is empty or contains an opponent's piece
            if ((board[newIndex].type == EMPTY || board[newIndex].color != King.color)) {
                possibleMoves.push_back(newIndex);
            }
        }
    }

    if ((King.color == Black && KingIndex == 4) || (King.color == White && KingIndex == 60)) {

        // Check for castling moves
        if (King.firstMove && !IsTileUnderAttack(KingIndex)) {
            // Check kingside castling
            if (canCastleKingSide(KingIndex)) {

                possibleMoves.push_back(KingIndex + 2);
            }
            // Check queenside castling
            if (canCastleQueenide(KingIndex)) {
                //cout << KingIndex << endl;
                possibleMoves.push_back(KingIndex - 2);
            }
        }
    }
}

bool ChessBoard::canCastleKingSide(int KingIndex) const {
    if (!board[KingIndex].canCastleKingSide) {
        return false;
    }

    int RookIndex = KingIndex + 3;
    if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)) {
        return false;
    }

    for (int i = KingIndex + 1; i < RookIndex; i++) {

        if (board[i].type != EMPTY || IsTileUnderAttack(i)) {

            return false;
        }

    }

    return true;
}

bool ChessBoard::canCastleQueenide(int KingIndex) const {
    if (!board[KingIndex].canCastleQueenSide) {
        return false;
    }

    int RookIndex = KingIndex - 4;
    if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)) {
        return false;
    }

    for (int i = KingIndex - 1; i > RookIndex; i--) {
        if (board[i].type != EMPTY || IsTileUnderAttack(i)) {
            return false;
        }
    }

    return true;
}

bool ChessBoard::isCheck(const ChessBoard& chessboard, int playerColor, string calledby) const {

    //cout << "Being Called by: " << calledby << endl;
    // Find the king's position
    int kingIndex = -100;
    for (int i = 0; i < Total_tiles; i++) {
        if (chessboard.board[i].type == KING && chessboard.board[i].color == playerColor) {
            kingIndex = i;
            break;
        }
    }

    if (kingIndex == -100) {
        cout << "\nError Code 404: King not found. \n\n";
        cout << calledby << "\n\n";
        exit(404);
        return false;
    }

    // Iterate through opponent's pieces and see if any can attack the king
    int opponentColor = (playerColor == White) ? Black : White;
    vector<int> possibleMoves;
    for (int i = 0; i < Total_tiles; i++) {
        if (chessboard.board[i].color == opponentColor) {
            if (chessboard.board[i].isSlidingPiece) {
                ComputeSlidingPieceMoves(i, possibleMoves); // Example function, change as needed
            }
            else if (chessboard.board[i].type == PAWN) {
                ComputePawnMoves(i, possibleMoves);
            }
            else if (chessboard.board[i].type == KNIGHT) {
                ComputeKnightMoves(i, possibleMoves);
            }
            else if (chessboard.board[i].type == KING) {
                ComputeKingMoves(i, possibleMoves);
            }

            // Check if any of the possible moves include the king's position
            for (int move : possibleMoves) {
                if (move == kingIndex) {
                    // PlaySound(KingChecked);
                    return true; // King is in check
                }
            }
        }
    }
    return false; // King is not in check
}

bool ChessBoard::isCheckmate(ChessBoard& chessboard, int playerColor) const {
    if (!isCheck(chessboard, playerColor, "board: Is Check Mate")) {
        return false; // King is not in check, so it can't be checkmate
    }

    vector<int> possibleMoves = GetAllPossibleMoves(playerColor);
    if (possibleMoves.size() > 0) {
        return false; // King can escape from check, so it's not checkmate
    }

    return true; // King is in checkmate
}

vector<int> ChessBoard::GetAllPossibleMoves(int playerColor) const {

    vector<int> possibleMoves;

    for (int i = 0; i < Total_tiles; i++) {
        if (board[i].color == playerColor) {
            vector<int> moves;

            // Compute moves based on the piece type
            if (board[i].isSlidingPiece) {
                ComputeSlidingPieceMoves(i, moves);
            }
            else if (board[i].type == PAWN) {
                ComputePawnMoves(i, moves);
            }
            else if (board[i].type == KNIGHT) {
                ComputeKnightMoves(i, moves);
            }
            else if (board[i].type == KING) {
                ComputeKingMoves(i, moves);
            }

            moves = FilterValidMoves(i, moves);
            for (auto move : moves) {
                possibleMoves.push_back(move);
            }
        }
    }

    return possibleMoves;
}

vector<string> ChessBoard::GetAllPossibleMovesInChessNotation(int playerColor) const {
    vector<string> possibleMoves;

    for (int i = 0; i < Total_tiles; i++) {
        if (board[i].color == playerColor) {
            vector<int> moves;

            // Compute moves based on the piece type
            if (board[i].isSlidingPiece) {
                ComputeSlidingPieceMoves(i, moves);
            }
            else if (board[i].type == PAWN) {
                ComputePawnMoves(i, moves);
            }
            else if (board[i].type == KNIGHT) {
                ComputeKnightMoves(i, moves);
            }
            else if (board[i].type == KING) {
                ComputeKingMoves(i, moves);
            }

            // Check if the move is legal (does not leave the king in check)
            for (int toTile : moves) {
                // Simulate the move
                ChessBoard tempBoard1163 = *this;
                tempBoard1163.MakeMove(i, toTile);

                // Check if the king is in check after the move
                if (!tempBoard1163.isCheck(tempBoard1163, playerColor, "board: Get all possible moves in chess notation")) {
                    // If not in check, add the move to possible moves
                    possibleMoves.push_back(ConvertToChessNotation(i, toTile));
                }
            }
        }
    }

    return possibleMoves;
}

string ChessBoard::ConvertToChessNotation(int fromTile, int toTile) const {
    string chessNotation = "";

    // Convert the 'fromTile' index to chess notation
    char fromCol = 'a' + (fromTile % 8);
    int fromRow = 8 - (fromTile / 8);
    chessNotation += fromCol;
    chessNotation += to_string(fromRow);

    // Add the 'toTile' index to chess notation
    char toCol = 'a' + (toTile % 8);
    int toRow = 8 - (toTile / 8);
    chessNotation += toCol;
    chessNotation += to_string(toRow);

    return chessNotation;
}

int ChessBoard::getAttacksOnSquare(int squareIndex, int opponentColor) const {
    int attacks = 0;
    vector<int> opponentMoves = GetAllPossibleMoves(opponentColor);
    for (int i = 0; i < opponentMoves.size(); ++i) {
        if (opponentMoves[i] == squareIndex) {
            attacks++;
        }
    }
    return attacks;
}

bool ChessBoard::IsTileUnderAttack(int squareIndex) const {

    for (int i = 0; i < OpponentMoves.size(); i++) {
        if (OpponentMoves[i] == squareIndex) {
            return true;
        }
    }
    return false;
}

void ChessBoard::DisplayMoves() {
    for (auto move : OpponentMoves) {
        cout << move << endl;
    }
}

vector<int> ChessBoard::GetAllPossibleMovesForPiece(int type, int index) const {
    vector<int> moves;

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
    isBoardReversed = false;
    for (int i = 0; i < Total_tiles; i++) {
        board[i].DestroyTextures();
    }
}

ChessPiece ChessBoard::GetPieceAtPosition(int index) const {
    return board[index];
}

vector<string> ChessBoard::GetAllCaptureMovesInChessNotation(int color) const {
    vector<string> captureMoves;

    for (int fromTile = 0; fromTile < Total_tiles; fromTile++) {
        ChessPiece piece = board[fromTile];
        if (piece.color == color) {
            for (int toTile = 0; toTile < Total_tiles; toTile++) {
                if (fromTile != toTile && isValidCaptureMove(fromTile, toTile)) {
                    string move = ConvertToChessNotation(fromTile, toTile);
                    captureMoves.push_back(move);
                }
            }
        }
    }

    return captureMoves;
}

bool ChessBoard::isValidCaptureMove(int fromTile, int toTile) const {
    ChessPiece fromPiece = board[fromTile];
    ChessPiece toPiece = board[toTile];

    // Check if there is a piece at the 'from' tile and it is of opposite color to the piece at the 'to' tile
    return fromPiece.type != EMPTY && toPiece.type != EMPTY && fromPiece.color != toPiece.color;
}

pair<int, int> ChessBoard::convertChessNotationToIndices(const string& move) const {
    int fromCol = move[0] - 'a';
    int fromRow = 7 - (move[1] - '1');
    int toCol = move[2] - 'a';
    int toRow = 7 - (move[3] - '1');
    int fromTile = fromRow * 8 + fromCol;
    int toTile = toRow * 8 + toCol;
    return make_pair(fromTile, toTile);
}

void ChessBoard::ComputeOpponentMoves() {
    OpponentMoves = GetAllPossibleMoves(isCurrentPlayerWhite() ? Black : White);
}


