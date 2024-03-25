#pragma once

#include "ChessPiece.h"


class Pawn : public ChessPiece{
public:
	Pawn(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(PAWN, pieceColor){}

};

class Rook : public ChessPiece {
public:
	Rook(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(ROOK, pieceColor) {}


};

class Queen : public ChessPiece {
public:
	Queen(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(QUEEN, pieceColor) {}


};

class Bishop : public ChessPiece {
public:
	Bishop(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(BISHOP, pieceColor) {}


};

class Knight : public ChessPiece {
public:
	Knight(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KNIGHT, pieceColor) {}


};

class King : public ChessPiece {




public:
	King(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KING, pieceColor) {}




};