#ifndef CONVERT_NOTATION_H
#define CONVERT_NOTATION_H


#include "../Other/GlobalVariables.h"


class ConvertNotation {
public:

    //Converts a Chess Notation to Indices, for e.g e2e4 --> <60, 44>
    std::pair<int, int> operator()(const std::string& move) const;

    //convert Chess Cordinates to Chess Notation
    std::string operator()(int fromTile, int toTile) const;

    //Converts Indices to Coordinates
    std::pair<int, int> operator()(int pieceIndex) const;
};

#endif // !CONVERT_NOTATION_H


