#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "BoardStats.h"


class GameModes {

    Flags flags;
    User Player;
    bool DoOnce;
    string FENString;



    float engine_depth;

public:
    BoardStats GameStats;
    ChessBoard chessboard;
    ChessEngine Horizon;
    //For settings
   
    char inputDepth[2] = { 0 }; 
    char feninput[75] = { 0 };
    int enteredDepth;
    string enteredFEN;

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

    void Settings();

    void FENSettings();

    void SetFENStrings(const string& fen);



};



#endif // !GAMEMODES_H





