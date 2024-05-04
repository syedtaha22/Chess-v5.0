#pragma once

#include "../Other/GlobalVariables.h"

//#include <utility>

class ConvertNotation{
public:

    //Converts a Chess Notation to Indices, for e.g e2e4 --> <60, 44>
    std::pair<int, int> operator()(const std::string& move) const;

    //convert Chess Cordinates to Chess Notation
    std::string operator()(int fromTile, int toTile) const;

    //Converts Indices to Coordinates
    pair<int, int> operator()(int pieceIndex) const;
};

