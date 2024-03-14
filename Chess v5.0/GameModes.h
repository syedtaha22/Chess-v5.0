#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "BoardStats.h"


class GameModes {


    BoardStats GameStats;
    Flags flags;
    User Player;
    bool DoOnce;
    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";

public:
    ChessBoard chessboard;
    ChessEngine Horizon;

    GameModes();

    void InitialiseMultiplayerMode();

    void Options();

    void HandleMoves(int PlayerColor);

    void MultiplayerMode();

    void ResetBoard(bool calculateELO);

    void RestartGame();

    void BackToMenu();

    void CalculateELO();

    void InitialiseSinglePlayerMode();

    void SinglePlayerMode();

    void DisplayBoard() const;

    void Destroy();

    void setFENstring(string newFen);

    void SaveTranspositions();

    void BoardSetUp();
};



#endif // !GAMEMODES_H





