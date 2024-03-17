#include "../../headers/Engine/TranspositionTables.h"


TranspositionTables::TranspositionTables() {
    SizeOfTranspostionTable = 0.0;
    initZobristKeys();
}

void TranspositionTables::initZobristKeys() {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            zobristKeys[i][j] = dis(gen);

        }
    }
}

void TranspositionTables::storeTranspositionTable(uint64_t hash, float score, int depth) {
    transpositionTable[hash] = { score, depth };
    
    ComputeSizeOfTranspositionTable();
}

void TranspositionTables::ComputeSizeOfTranspositionTable() {

    size_t pairSize = sizeof(pair<const uint64_t, pair<float, int>>);                        // Size of each key-value pair in bytes
    size_t numPairs = transpositionTable.size();                                             // Total number of pairs in the map
    size_t totalPairSize = pairSize * numPairs;                                              // Total size of pairs in bytes
    size_t overhead = sizeof(transpositionTable);                                            // Overhead of the unordered_map itself
    SizeOfTranspostionTable = static_cast<double>(totalPairSize + overhead) / (1024 * 1024); // Convert bytes to megabytes


}

bool TranspositionTables::isValuePresent(uint64_t hash) {

    for (const auto& key : transpositionTable) {
        if (key.first == hash) {
            return true;
        }
    }
    return false;
}

void TranspositionTables::saveTranspositionTableToFile(string filename) {
    string folder = "Data";

    if (!filesystem::is_directory(folder)) {
        filesystem::create_directory(folder);
    }

    filename = "Data/" + filename;

    ofstream outputFile(filename);
    int MaxLenght = 20;
    if (outputFile.is_open()) {
        for (const auto& entry : transpositionTable) {
            outputFile << left << setw(MaxLenght) << entry.first << right << setw(5) << entry.second.second << "D   " << entry.second.first << endl;
        }
        outputFile.close();
        cout << "Saved Transposition Tables to: " << filename << endl;
    }
    else {
        cerr << "Error: Unable to open file for writing." << endl;
    }


}

uint64_t TranspositionTables::computeHash(const ChessBoard& board) const {
    uint64_t hash = 0;
    for (int i = 0; i < 64; ++i) {
        ChessPiece piece = board.board[i];
        if (piece.type != EMPTY) {
            int index = piece.PieceCode;
            hash ^= zobristKeys[i][index];
        }
    }
    return hash;
}

pair<float, int> TranspositionTables::lookupTranspositionTable(uint64_t hash) const {
    auto it = transpositionTable.find(hash);
    if (it != transpositionTable.end()) {
        return it->second;
    }
    return { -INFINITY, -1 }; // Not found
}
