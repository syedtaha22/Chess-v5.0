#include "../../headers/Engine/EngineState.h"



ChessEngineStatistics Heuristics;

EngineState::EngineState() {
    engineEloRating = 500;
    MAX_DEPTH = 2;

    if (MAX_DEPTH == 0) MAX_DEPTH = 2; //Default

    EngineColor = EMPTY;

    terminateSearch = false;
    startSearch = false;
}

void EngineState::LoadSavedSettings() {
    engineEloRating = getELOFromSettings();

    MAX_DEPTH = Settings::get(Settings::depth);
    if (MAX_DEPTH == 0) MAX_DEPTH = 2; //Default
}

void EngineState::setEngineColor(int color) { EngineColor = color; }

int EngineState::getELOFromSettings() const {
    return (Settings::get(Settings::engineElo) != 0) ? Settings::get(Settings::engineElo) : 500;
}

int EngineState::getELO() const {
    return engineEloRating;
}


void EngineState::setElO(int newELO) {
    engineEloRating = newELO;
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
    try {
        //DebugItem()(pieceValues.at(index));
        return pieceValues.at(index);
    }
    catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Unknown Piece type: " << index << " found\n";
        return 0;
    }
}

void EngineState::reset() {
    Heuristics.reset();
    terminateSearch = true;
    startSearch = false;
}

int EngineState::GetEngineColor() const {
    return EngineColor;
}

float EngineState::getTimeLimit() const {
    return timeLimit;
}