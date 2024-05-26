#pragma once

#include "../Functors/ChessDebugger.h"



struct BoardState{

    //PreComputed Moves for Pieces
    std::vector<int> SlidingPieceMoveData;
    std::vector<int> PawnMoveData;
    std::vector<int> KnightMoveData;
    std::vector<int> KingMoveData;

    //PreComputed Moves for Opponents
    std::vector<int> OpponentMoves;

    //Precomputes Move for Selected Piece by User
    std::vector<int> MovesForSelectedPiece;

    //Contains a History of all Moves Mades
    std::vector<std::string> moveHistory;

    //Stores the last Move that was made in the Format
    //<Initial Index, Final Index>
    std::pair<int, int> MoveIndices;

    //Returns true if it is Whites Turn To Play
    bool currentPlayerIsWhite;

    //Determines wether the Move made is a Castling Move or not
    bool DoCastle;

    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 10;

    //Stores Index of Pawn that is Capturable by En Passant
    int enPassantTarget;

    //Determines wether a board is Reversed or Not.(For Multiplayer Mode)
    bool isBoardReversed;

    //Wether the Move made was A Captured Move or not
    bool PieceIsCaptured;
    int checkedPlayer;

    BoardState() {
        currentPlayerIsWhite = true;
        DoCastle = false;
        MoveIndices = { -1, -1 };
        isBoardReversed = false;
        PieceIsCaptured = false;
        enPassantTarget = -1;
        checkedPlayer = 0;
    }

    BoardState& operator=(const BoardState& other) {
        if (this != &other) { // Check for self-assignment
            // Perform the assignment
            currentPlayerIsWhite = other.currentPlayerIsWhite;
            DoCastle = other.DoCastle;
            MoveIndices = other.MoveIndices;
            isBoardReversed = other.isBoardReversed;
            PieceIsCaptured = other.PieceIsCaptured;
            enPassantTarget = other.enPassantTarget;
            checkedPlayer = other.checkedPlayer;
        }
        return *this; // Return a reference to the modified object
    }

    int getCurrentPlayer() const {
        return currentPlayerIsWhite ? White : Black;
    }

    void reset() {
        isBoardReversed = false;
        moveHistory.clear();
        MovesForSelectedPiece.clear();
        OpponentMoves.clear();
    }

};

