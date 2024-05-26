#pragma once

#include "PieceSquareTables.h"

extern PieceSquareTables PST;

class Pawn : public ChessPiece{
public:
	Pawn(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(PAWN, pieceColor){}
	int getScore() const override{ return pawnScore;}

	virtual Pawn* clone() override {
		return new Pawn(this->color, this->firstMove);
	}

};

class Rook : public ChessPiece {
public:
	Rook(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(ROOK, pieceColor) {}
	int getScore() const override { return rookScore; }

	virtual Rook* clone() override {
		return new Rook(this->color, this->firstMove);
	}
};

class Queen : public ChessPiece {
public:
	Queen(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(QUEEN, pieceColor) {}
	int getScore() const override { return queenScore; }

	virtual Queen* clone() override {
		return new Queen(this->color, this->firstMove);
	}
};

class Bishop : public ChessPiece {
public:
	Bishop(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(BISHOP, pieceColor) {}
	int getScore() const override { return bishopScore; }

	virtual Bishop* clone() override {
		return new Bishop(this->color, this->firstMove);
	}
};

class Knight : public ChessPiece {
public:
	Knight(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KNIGHT, pieceColor) {}
	int getScore() const override { return knightScore; }

	virtual Knight* clone() override {
		return new Knight(this->color, this->firstMove);
	}
};

class King : public ChessPiece {
public:
	King(int pieceColor = EMPTY, bool isFirstMove = true) : ChessPiece(KING, pieceColor) {}
	int getScore() const override { return kingScore; }

	virtual King* clone() override {
		return new King(this->color, this->firstMove);
	}
};

