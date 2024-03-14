#ifndef TRANSPOSITIONTABLES_H
#define TRANSPOSITIONTABLES_H

#include "../Board/ChessBoard.h"

class TranspositionTables {

    uint64_t zobristKeys[64][12]; // 64 squares, 12 piece types
    unordered_map<uint64_t, pair<float, int>> transpositionTable; // <hash, <score, depth>>


public:
    double SizeOfTranspostionTable;

    TranspositionTables();

    void initZobristKeys();

    // Compute hash value for the current board position
    uint64_t computeHash(const ChessBoard& board) const;


    // Search in transposition table
    pair<float, int> lookupTranspositionTable(uint64_t hash) const;

    // Store in transposition table
    void storeTranspositionTable(uint64_t hash, float score, int depth);

    void ComputeSizeOfTranspositionTable();

    bool isValuePresent(uint64_t hash);


    void saveTranspositionTableToFile(const string& filename);


};

#endif // !TRANSPOSITIONTABLES_H


