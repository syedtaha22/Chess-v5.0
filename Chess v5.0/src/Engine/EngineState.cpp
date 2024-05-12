#include "../../headers/Engine/EngineState.h"



ChessEngineStatistics Heuristics;

EngineState::EngineState() {
    engineEloRating = getELO();
    MAX_DEPTH = Settings::get("depth");
    if (MAX_DEPTH == 0) MAX_DEPTH = 2; //Default

    terminateSearch = false;
    startSearch = false;
}

void EngineState::setEngineColor(int color) { EngineColor = color; }

int EngineState::getELO() {
    return (Settings::get("engineElo") != 0) ? Settings::get("engineElo") : 500;
}

void EngineState::StartSearch() { startSearch = true; }

void EngineState::StopSearching() { startSearch = false; }

void EngineState::TerminateSearch() { terminateSearch = true; }

bool EngineState::isSearchStarted() const { return startSearch && !terminateSearch; }

void EngineState::SaveTranspositionTable() {
    transpostionTable.saveTranspositionTableToFile("TranspositionTables.txt");
}

double EngineState::getSizeOfTranspositionTable() const {
    return transpostionTable.SizeOfTranspostionTable;
}

int EngineState::getDepth() const { return MAX_DEPTH; }

void EngineState::setDepth(int NewDepth) { MAX_DEPTH = NewDepth; }

int EngineState::getPieceValue(int index) const {
    auto it = pieceValues.find(index);
    if (it != pieceValues.end()) {
        return it->second;
    } else {
        std::cerr << "Unknown Piece type with code " << index << " found\n";
        return 0;
    }
}

void EngineState::reset() {
    Heuristics.reset();
    terminateSearch = true;
    startSearch = false;
}
