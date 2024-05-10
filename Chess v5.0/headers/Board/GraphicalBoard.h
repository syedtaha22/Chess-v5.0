#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include "ChessBoard.h"

class GraphicalBoard {
public:

    //Draws the Board Tiles
    void DrawBoard(const ChessBoard& chessboard) const;

    //Draws the Coordinates oñthe board
    void DrawCoordinates(const ChessBoard& chessboard) const;

    //Display the Indices of the squares on the board
    void DrawSquareIndices(const ChessBoard& chessboard) const;

    //Draws the ChessBoard
    void DrawChessPiece(const ChessBoard& chessboard) const;
};

#endif // !GRAPHICALBOARD_H