#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include "ChessBoard.h"

class GraphicalBoard {

    const float TextSize = 15;

    // contains Precalculated Indices and the coordinates that need to be drawn,
    // <index, <coordinate, isRow>>;  
    std::vector<std::pair<int, std::pair<std::string, bool>>> CoordinateIndexPairs; 

    //Initialise and PreStore Tiles where Coordinates need to be drawn
    void initCoordinatesIndexPairs();


    //Calculates the Postion where Coordinates need to be drawn    
    Vector2 getPosition(bool isRow, int LocX, int LocY) const;

public:

    GraphicalBoard();

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