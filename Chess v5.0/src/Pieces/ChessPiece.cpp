#include "../../headers/Pieces/ChessPiece.h"

ChessPiece::ChessPiece(int pieceType, int pieceColor, bool isFirstMove) : type(pieceType), color(pieceColor), firstMove(true) {

    PieceCode = pieceColor | pieceType;
    isSlidingPiece = (type == ROOK) || (type == BISHOP) || (type == QUEEN);
    rectangle.width = rectangle.height = static_cast<float>(tileSize);
    isDragged = false;
    canCastleQueenSide = false;
    canCastleKingSide = false;
    EnpassantTarget = false;
    texture = {0};


}

void ChessPiece::AssignTextures() {

    if (type != EMPTY) {
        std::string fileName = "Resources\\";
        fileName += (color == White) ? "WhitePieces\\" : "BlackPieces\\";
        int temp;
        temp = color | type;
        fileName += std::to_string(PieceCode);
        fileName += ".png";
        texture = LoadTexture(fileName.c_str());
    }
}

void ChessPiece::DestroyTextures() {
    if (type != EMPTY) {
        std::cout << "Unloaded Texture for: " << color << " " << type << std::endl;
        UnloadTexture(texture);
    }
}
