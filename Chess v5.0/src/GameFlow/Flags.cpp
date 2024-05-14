#include "../../headers/GameFlow/Flags.h"


void Flags::CheckGameState() {isMoveMade = true;}

void Flags::GameStateChecked() {isMoveMade = false;}

bool Flags::MoveIsMade() {return Flags::isMoveMade;}

void Flags::StartGame() {gameStarted = true;}

bool Flags::isMultiplayerGame() {return MultiplayerGame;}

bool Flags::isSinglePlayer() {return SinglePlayer;}

bool Flags::isGameStarted() {return gameStarted;}

void Flags::EndGame() {
    gameStarted = false;
    SinglePlayer = false;
    MultiplayerGame = false;
}

void Flags::SinglePlayerMode() {SinglePlayer = true;}

void Flags::MultiplayerMode() {MultiplayerGame = true;}

void Flags::DisableSinglePlayer() { SinglePlayer = false; }

void Flags::OpenSettings() { showSettings = true; }

bool Flags::SettingsOpened() { return showSettings; }

void Flags::closeSettings() { showSettings = false; }

void Flags::OpenFENSettings() { SetFEN = true; }

bool Flags::isFENSettingsOpened() { return SetFEN; }

void Flags::closeFENSettings() { SetFEN = false; }

bool Flags::isMoveMade = false;
bool Flags::SinglePlayer = false;
bool Flags::MultiplayerGame = false;
bool Flags::gameStarted = false;
bool Flags::showSettings = false;
bool Flags::SetFEN = false;
