#ifndef CHECK_AFTER_MOVE_H
#define CHECK_AFTER_MOVE_H

#include "../Board/GraphicalBoard.h"

class CheckAfterMove {
public:
    bool operator()(const std::string& move, const ChessBoard& board, int color) const {
        ChessBoard tempBoard(board);
        std::pair<int, int> indices = ConvertNotation()(move);
        tempBoard.MakeMove(indices.first, indices.second);

        // Check if the move puts the opponent's king in check
        return tempBoard.isCheck(tempBoard, (color == White ? White : Black), "Engine: Check after move");
    }
};

#endif // !CHECK_AFTER_MOVE_H