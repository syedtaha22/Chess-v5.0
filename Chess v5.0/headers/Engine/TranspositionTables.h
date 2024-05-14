#ifndef TRANSPOSITIONTABLES_H
#define TRANSPOSITIONTABLES_H

#include "../Functors/isCaptureMove.h"



class TranspositionTables {
private:

    /*
        ZobristKeys         ---->[NumberOFSquares][Number of PieceTypes]
        Transpostion Tables ----> <hash, <score, depth>>

    */

    uint64_t zobristKeys[64][12]; 
    std::unordered_map<uint64_t, std::pair<int, int>> transpositionTable;

public:
    double SizeOfTranspostionTable;

    TranspositionTables();

    //Initialise Zobrist Keys
    void initZobristKeys();

    //Save a Traspostion Table
    void storeTranspositionTable(uint64_t hash, int score, int depth);
    void ComputeSizeOfTranspositionTable();

    //For Debugging and Testing, save The Transpostions to a File
    void saveTranspositionTableToFile(std::string filename);

    //Compute Hash value
    uint64_t computeHash(const ChessBoard& board) const;

    std::pair<int, int> lookupTranspositionTable(uint64_t hash) const;

    bool isValuePresent(uint64_t hash);

};

#endif // !TRANSPOSITIONTABLES_H


