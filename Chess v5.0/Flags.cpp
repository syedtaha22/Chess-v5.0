#include "Flags.h"


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


