#pragma once

#include "../Pieces/Pieces.h"


class DebugItem {
public:

    //Display Vector on Terminal
	void operator()(const std::vector<std::string>& moves) {
		for (const auto& move : moves) { std::cout << move << " "; }
		std::cout << std::endl;
	}

    //Display Board on Terminal
    void operator()(ChessPiece* board[]) const {

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

    // Base case function to print the last argument
    void print_args(std::ostream& os) {
        os << std::endl;
    }

    // Recursive variadic template function to print arguments
    template<typename T, typename... Args>
    void print_args(std::ostream& os, const T& firstArg, const Args&... args) {
        os << firstArg << " ";
        print_args(os, args...); // Recur for the remaining arguments
    }

    // Wrapper function to allow usage with std::cout
    template<typename... Args>
    void operator()(Args&&... args) {
        print_args(std::cout, std::forward<Args>(args)...);
    }

};



