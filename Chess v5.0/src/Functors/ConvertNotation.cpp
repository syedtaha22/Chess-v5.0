#include "../../headers/Functors/ConvertNotation.h"

std::pair<int, int> ConvertNotation::operator()(const std::string& move) const {
    int fromCol = move[0] - 'a';
    int fromRow = 7 - (move[1] - '1');
    int toCol = move[2] - 'a';
    int toRow = 7 - (move[3] - '1');
    int fromTile = fromRow * 8 + fromCol;
    int toTile = toRow * 8 + toCol;
    return std::make_pair(fromTile, toTile);
}

std::string ConvertNotation::operator()(int fromTile, int toTile) const {
    std::string chessNotation = "";

    // Convert the 'fromTile' index to chess notation
    char fromCol = 'a' + (fromTile % 8);
    int fromRow = 8 - (fromTile / 8);
    chessNotation += fromCol;
    chessNotation += std::to_string(fromRow);

    // Add the 'toTile' index to chess notation
    char toCol = 'a' + (toTile % 8);
    int toRow = 8 - (toTile / 8);
    chessNotation += toCol;
    chessNotation += std::to_string(toRow);

    return chessNotation;
}

pair<int, int> ConvertNotation::operator()(int pieceIndex) const {
    if (pieceIndex < 0 || pieceIndex >= Total_tiles) {
        return make_pair(-1, -1);
    }
    int rank = pieceIndex / boardSize;
    int file = pieceIndex % boardSize;
    return make_pair(rank, file);
}



