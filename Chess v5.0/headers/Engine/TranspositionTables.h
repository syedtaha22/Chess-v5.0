#ifndef TRANSPOSITIONTABLES_H
#define TRANSPOSITIONTABLES_H

#include "../Board/ChessBoard.h"

class TranspositionTables {

    /*
        ZobristKeys         ---->[NumberOFSquares][Number of PieceTypes]
        Transpostion Tables ----> <hash, <score, depth>>

    */

    uint64_t zobristKeys[64][12]; 
    unordered_map<uint64_t, pair<int, int>> transpositionTable;
    const int infinity = numeric_limits<int>::max();

public:
    double SizeOfTranspostionTable;

    TranspositionTables();

    void initZobristKeys();
    void storeTranspositionTable(uint64_t hash, int score, int depth);
    void ComputeSizeOfTranspositionTable();
    void saveTranspositionTableToFile(string filename);

    uint64_t computeHash(const ChessBoard& board) const;

    pair<int, int> lookupTranspositionTable(uint64_t hash) const;

    bool isValuePresent(uint64_t hash);

};

#endif // !TRANSPOSITIONTABLES_H


