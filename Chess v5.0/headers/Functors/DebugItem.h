#pragma once

#include "../Pieces/Pieces.h"


class DebugItem {
public:

	void operator()(const std::vector<std::string>& moves) {
		for (const auto& move : moves) { std::cout << move << " "; }
		std::cout << std::endl;
	}

    void operator()(ChessPiece* board[]) const {
        //Display Board on Terminal. For Debugging Purposes

        std::cout << "8 ";
        for (int i = 0; i < Total_tiles; i++) {
            // cout << (i/8) + 1  << " ";
            std::cout << "[" << board[i]->type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles) {
                std::cout << std::endl;
                std::cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        std::cout << std::endl;
        std::cout << "   a  b  c  d  e  f  g  h" << std::endl;
    }

};



