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

void EngineState::initPieceValues() {
    // Piece values for evaluation
    if (EngineColor == White) {
        pieceValues = {
            {White | PAWN,  100}, {White | KNIGHT,  320}, {White | BISHOP,  330}, {White | ROOK,  500}, {White | QUEEN,  900}, {White | KING,  infinity},
            {Black | PAWN, -100}, {Black | KNIGHT, -320}, {Black | BISHOP, -330}, {Black | ROOK, -500}, {Black | QUEEN, -900}, {Black | KING, -infinity} };
    }
    else {
        pieceValues = {
            {White | PAWN, -100}, {White | KNIGHT, -320}, {White | BISHOP, -330}, {White | ROOK, -500}, {White | QUEEN, -900}, {White | KING, -infinity},
            {Black | PAWN,  100}, {Black | KNIGHT,  320}, {Black | BISHOP,  330}, {Black | ROOK,  500}, {Black | QUEEN,  900}, {Black | KING,  infinity} };
    }
}

int EngineState::getELO() {
    return (Settings::get("engineElo") != 0) ? Settings::get("engineElo") : 500;
}

void EngineState::StartSearch() { startSearch = true; }

void EngineState::StopSearching() { startSearch = false; }

void EngineState::TerminateSearch() { terminateSearch = true; }

bool EngineState::isSearchStarted() const { return startSearch; }

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

