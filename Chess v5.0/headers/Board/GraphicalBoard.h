#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include "ChessBoard.h"

class GraphicalBoard {
    const Color lightSquare = { 191, 208, 204, 255 }; //Light Green
    const Color darkSquare = { 66, 138, 152, 255 }; //Teal

    //const Color lightSquare = { 238, 238, 210, 255 }; //Light Yellow
    //const Color darkSquare = { 118, 150, 86, 255 }; // Green

    const Color MoveHighlight = Fade({ 255, 255, 0, 200 }, 0.5);
    const Color MovesForPieceHighLight = Fade({ 255, 0, 0, 200 }, 0.5);
    const Color MoveHighlightBlue = Fade({ 0, 0, 255, 200 }, 0.5);
    const Color NextMoveHighlight = Fade({ 0, 255, 0, 200 }, 0.5);

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