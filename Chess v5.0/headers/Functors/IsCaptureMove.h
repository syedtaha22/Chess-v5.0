#ifndef IS_CAPTURE_MOVE_H
#define IS_CAPTURE_MOVE_H

#include "CheckAfterMove.h"

class IsCaptureMove {
public:

    bool operator()(const std::string& move, const ChessBoard board) const {
        std::pair<int, int> indices = ConvertNotation()(move);
        ChessPiece* targetPiece = board.GetPieceAtPosition(indices.second);

        // Check if the target piece is an opponent's piece
        return targetPiece->color != board.GetPieceAtPosition(indices.first)->color && targetPiece->color != EMPTY;
    }

};


#endif // !IS_CAPTURE_MOVE_H






