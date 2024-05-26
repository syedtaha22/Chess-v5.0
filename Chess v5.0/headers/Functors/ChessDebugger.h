#ifndef CHESS_DEBUGGER_H
#define CHESS_DEBUGGER_H

#include "../Pieces/Pieces.h"


class ChessDebugger {
    // Base case function to print the last argument
    void print() {
        std::cout << std::endl;
    }



public:
    // Display Vector on Terminal
    void moves(const std::vector<std::string>& moves) {
        for (const auto& move : moves) {
            std::cout << move << " ";
        }
        std::cout << std::endl;
    }

 
    // Wrapper function to allow usage with std::cout
    // Recursive variadic template function to print arguments
    template<typename T, typename... Args>
    void print(const T& firstArg, const Args&... args) {
        std::cout << firstArg << " ";
        print(args...); // Recur for the remaining arguments
    }

};

#endif // !CHESS_DEBUGGER_H