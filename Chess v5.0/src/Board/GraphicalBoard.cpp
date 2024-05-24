#include "../../headers/Board/GraphicalBoard.h"


GraphicalBoard::GraphicalBoard() {
    initCoordinatesIndexPairs();
}

void GraphicalBoard::initCoordinatesIndexPairs() {
    CoordinateIndexPairs.reserve(16);

    // Row Coordinates(1-8)
    for (int i = 0; i < 64; i += 8) {
        CoordinateIndexPairs.emplace_back(std::make_pair(i, std::make_pair(std::to_string(8 - i / 8), true)));
    }

    // Column Coordinates(a-h)
    for (int i = 56; i < 64; ++i) {
        std::string str = "";
        str += 'a' + i % 8;
        CoordinateIndexPairs.emplace_back(std::make_pair(i, std::make_pair(str, false)));
    }

}

void GraphicalBoard::DrawBoard(const ChessBoard& chessboard) const {
    
    for (int index = 0; index < Total_tiles; index++) {
        std::pair<int, int> PieceCoords = ConvertNotation()(index);
        Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

        int LocationX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocationY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        //Draw The Board
        DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);


        // Highlight Last Move
        if (chessboard.state.MoveIndices.first == index || chessboard.state.MoveIndices.second == index) {
            DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
        }

        // if Current Index is a Move for Selected Piece than Highlight it
        if (std::binary_search(chessboard.state.MovesForSelectedPiece.begin(), chessboard.state.MovesForSelectedPiece.end(), index)) {
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight); //Displays Red Squares
        }
    }
}

void GraphicalBoard::DrawCoordinates(const ChessBoard& chessboard) const {
    
    for (const auto& square : CoordinateIndexPairs) {
        std::pair<int, int> PieceCoords = ConvertNotation()(square.first);
        int LocX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        bool ColorCondition = ((PieceCoords.first + PieceCoords.second) % 2 == 0 && !chessboard.state.isBoardReversed) ||
            ((PieceCoords.first + PieceCoords.second) % 2 != 0 && chessboard.state.isBoardReversed);

        Color TextColor = (ColorCondition) ? darkSquare : lightSquare;
        Vector2 Position = getPosition(square.second.second, LocX, LocY);
        
        DrawTextEx(myFont, square.second.first.c_str(), Position, TextSize, 0.1f, TextColor);

    }
}

Vector2 GraphicalBoard::getPosition(bool isRow, int LocX, int LocY) const {
    /*
        In Row: X postion is Constant, the +8 is an offset from corner of tile
            return { 800 + 8, static_cast<float>(LocY) + 8 };
        In Column: Y postion is Constant, the -20 is an offset from corner of tile
            return { static_cast<float>(LocX + tileSize) - 20, 720 - 20 };
    */
    return (isRow) ? Vector2(800 + 8, static_cast<float>(LocY) + 8)
        : Vector2(static_cast<float>(LocX + tileSize) - 20, 720 - 20);
}

void GraphicalBoard::DrawSquareIndices(const ChessBoard& chessboard) const {
    //For Debugging Purposes, Draws tile index
    for (int index = 0; index < Total_tiles; index++) {
        std::pair<int, int> PieceCoords = ConvertNotation()(index);
        int LocationX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocationY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;
        DrawText(std::to_string(index).c_str(), LocationX + 8, LocationY + 8, 10, RED);
    }
}

void GraphicalBoard::DrawChessPiece(const ChessBoard& chessboard) const {
    for (int index = 0; index < Total_tiles; index++) {
        int PositionX = static_cast<int>(chessboard.board[index]->rectangle.x);
        int PositionY = static_cast<int>(chessboard.board[index]->rectangle.y);
        DrawTexture(chessboard.board[index]->texture, PositionX, PositionY, WHITE);
    }
}