#include "../../headers/Board/GraphicalBoard.h"

void GraphicalBoard::DrawBoard(const ChessBoard& chessboard) const {
    for (int index = 0; index < Total_tiles; index++) {
        pair<int, int> PieceCoords = ConvertNotation()(index);
        Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

        int LocationX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocationY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        //Draw The Board
        DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);


        // Highlight Last Move
        if (chessboard.state.MoveIndices.first == index || chessboard.state.MoveIndices.second == index) {
            DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
        }

        // Show Moves for Piece
        for (const auto& move : chessboard.state.MovesForSelectedPiece) {
            if (move == index) {
                float Diameter = 26;
                float Radius = Diameter / 2;
                float Offset = Diameter + Radius;

                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight); //Displays Red Squares

                //Uncomment Below Line to get Circular Indicators
                //DrawCircle(LocationX + static_cast<int>(Offset), LocationY + static_cast<int>(Offset), Radius, Translucent);
            }
        }

    }
}

void GraphicalBoard::DrawCoordinates(const ChessBoard& chessboard) const {
    const int TextSize = 15;

    for (int index = 0; index < Total_tiles; index++) {
        pair<int, int> PieceCoords = ConvertNotation()(index);
        int LocX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        // Condition to Decide, Font Color. 
        // if is Divisible by 2 and board is not reversed ---> darkSquare(color)
        // if is not Divisible by 2 and board is reversed ---> darkSquare(color)
        // otherwise lightSqaure(color)

        bool ColorCondition = ((PieceCoords.first + PieceCoords.second) % 2 == 0 && !chessboard.state.isBoardReversed) ||
            ((PieceCoords.first + PieceCoords.second) % 2 != 0 && chessboard.state.isBoardReversed);

        Color TextColor = (ColorCondition) ? darkSquare : lightSquare;

        // Row Coordinates(1-8)
        if (index % 8 == 0) {
            // X postion is Constant, the +8 is an offset from corner of tile
            Vector2 Position = { 800 + 8, static_cast<float>(LocY) + 8 };

            DrawTextEx(myFont, to_string(8 - index / 8).c_str(), Position, TextSize, 0.1f, TextColor);
        }

        // Column Coordinates(a-h)
        if (index >= 56 && index <= 63) {
            // Y postion is Constant, the -20 is an offset from corner of tile
            Vector2 Position = { static_cast<float>(LocX + tileSize) - 20, 720 - 20 };

            DrawTextEx(myFont, string(1, 'a' + index % 8).c_str(), Position, TextSize, 0.1f, TextColor);
        }
    }
}

void GraphicalBoard::DrawSquareIndices(const ChessBoard& chessboard) const {
    //For Debugging Purposes, Draws tile index
    for (int index = 0; index < Total_tiles; index++) {
        pair<int, int> PieceCoords = ConvertNotation()(index);

        int LocationX = BoardOffsetX + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
        int LocationY = BoardOffsetY + (abs((chessboard.state.isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

        DrawText(to_string(index).c_str(), LocationX + 8, LocationY + 8, 10, RED);
    }

}

void GraphicalBoard::DrawChessPiece(const ChessBoard& chessboard) const {
    for (int index = 0; index < Total_tiles; index++) {
        auto something = chessboard.board[index]->rectangle.x;
        int PositionX = static_cast<int>(chessboard.board[index]->rectangle.x);
        int PositionY = static_cast<int>(chessboard.board[index]->rectangle.y);
        DrawTexture(chessboard.board[index]->texture, PositionX, PositionY, WHITE);
    }
}


