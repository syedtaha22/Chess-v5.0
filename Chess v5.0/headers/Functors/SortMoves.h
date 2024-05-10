#ifndef SORT_MOVES_H
#define SORT_MOVES_H

#include "isCaptureMove.h"

class SortMoves {
public:

    void operator()(std::vector<std::string>& moves, const ChessBoard& board, int color) const {

        // Define custom comparator to sort moves
        auto customComparator = [&](const std::string& move1, const std::string& move2) {
            // Check if move1 gives check
            bool check1 = CheckAfterMove()(move1, board, color);

            // Check if move2 gives check
            bool check2 = CheckAfterMove()(move2, board, color);

            // If one move gives check and the other doesn't, prioritize the move that gives check
            if (check1 != check2)
                return check1;

            // If engineh moves give check or engineh moves don't give check, prioritize captures
            bool capture1 = IsCaptureMove()(move1, board);
            bool capture2 = IsCaptureMove()(move2, board);

            return capture1 > capture2; // Sorting captures before non-captures
            };

        // Sort moves using the custom comparator
        std::sort(moves.begin(), moves.end(), customComparator);
    }

};

#endif // !SORT_MOVES_H




