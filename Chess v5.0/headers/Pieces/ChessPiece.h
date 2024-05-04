#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "../Functors/ConvertNotation.h"
#include "PieceCodes.h"

class ChessPiece {
public:
    int type; 
    int color;
    bool firstMove;
    bool isSlidingPiece;
    Texture2D texture;
    Rectangle rectangle;
    bool isDragged;
 
    bool canCastleQueenSide;
    bool canCastleKingSide;
    int PieceCode;
    int EnpassantTarget;

    /*
    -----------------------------Piece Type Codes-----------------------------

                                White King   = 9
                                White Pawn   = 10
                                White Knigh  = 11
                                White Bishop = 12
                                White Rook   = 13
                                White Queen  = 14   

                                Black King   = 17
                                Black Pawn   = 18
                                Black Knigh  = 19
                                Black Bishop = 20
                                Black Rook   = 21
                                Black Queen  = 22
    */

    ChessPiece(int pieceType = EMPTY, int pieceColor = EMPTY, bool isFirstMove = true);

    //Loads Textures Based on PieceCodes
    void AssignTextures();
    //Unloads textures associated to the Piece
    void DestroyTextures();

    

    //Not Using Destructer here as It, Unexpectedly Destroys Pieces.
    //~ChessPiece(){}
};


#endif // !CHESSPIECE_H


