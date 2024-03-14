#include "ChessPiece.h"

ChessPiece::ChessPiece(int pieceType, int pieceColor, bool isFirstMove) : type(pieceType), color(pieceColor), firstMove(true) {

    PieceCode = pieceColor | pieceType;
    isSlidingPiece = (type == ROOK) || (type == BISHOP) || (type == QUEEN);
    AssignTextures();
    rectangle.width = tileSize;
    rectangle.height = tileSize;
    isDragged = false;
    canCastleQueenSide = false;
    canCastleKingSide = false;


}

void ChessPiece::AssignTextures() {

    if (type != EMPTY) {
        string fileName = "Resources\\";
        fileName += (color == White) ? "WhitePieces\\" : "BlackPieces\\";
        int temp;
        temp = color | type;
        fileName += to_string(PieceCode);
        fileName += ".png";
        texture = LoadTexture(fileName.c_str());
    }
}

void ChessPiece::DestroyTextures() {
    if (type != EMPTY) {
        cout << "Unloaded Texture for: " << color << " " << type << endl;
        UnloadTexture(texture);
    }
}
