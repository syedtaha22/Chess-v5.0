#ifndef CHESS_DEBUGGER_H
#define CHESS_DEBUGGER_H

#include "../Pieces/Pieces.h"


class ChessDebugger {
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

public:

    //Display Vector on Terminal
    static void moves(const std::vector<std::string>& moves_) {
        for (const auto& move : moves_) { std::cout << move << " "; }
        std::cout << std::endl;
    }


    // Wrapper function to allow usage with std::cout
    template<typename... Args>
    void operator()(Args&&... args) {
        print_args(std::cout, std::forward<Args>(args)...);
    }

};

#endif // !CHESS_DEBUGGER_H