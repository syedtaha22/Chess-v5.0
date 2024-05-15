#pragma once

#include "PieceSquareTables.h"

extern PieceSquareTables PST;

class Pawn : public ChessPiece{
public:
	Pawn(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(PAWN, pieceColor){}
	int getScore() const override{ return pawnScore;}
};

class Rook : public ChessPiece {
public:
	Rook(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(ROOK, pieceColor) {}
	int getScore() const override { return rookScore; }
};

class Queen : public ChessPiece {
public:
	Queen(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(QUEEN, pieceColor) {}
	int getScore() const override { return queenScore; }
};

class Bishop : public ChessPiece {
public:
	Bishop(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(BISHOP, pieceColor) {}
	int getScore() const override { return bishopScore; }
};

class Knight : public ChessPiece {
public:
	Knight(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KNIGHT, pieceColor) {}
	int getScore() const override { return knightScore; }
};

class King : public ChessPiece {
public:
	King(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KING, pieceColor) {}
	int getScore() const override { return kingScore; }
};

