#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "../Statistics/BoardStats.h"
#include "Menu.h"
#include "../Functors/CalculateELO.h"

class GameModes {
private:
    //Things that will be done once per match
    bool DoOnce;

    std::string FENString;

    GraphicalBoard graphics;
    BoardStats GameStats;
    Menu GameMenu;

    float engine_depth;

public:
    //Game Related Objects
    User Player;
    ChessBoard chessboard;
    ChessEngine Horizon;


    //For settings
    char inputDepth[2] = { 0 }; 
    char feninput[75] = { 0 };
    int enteredDepth;
    std::string enteredFEN;

    GameModes();

    void Options();

    //Set up Multiplayer Mode
    void InitialiseMultiplayerMode();
    //Logic for Multiplayer Mode
    void MultiplayerMode();

    //Set up Single Player Mode
    void InitialiseSinglePlayerMode();
    //Logic for Single Player Mode
    void SinglePlayerMode();

    //Check if the player is Making a Move and Handle that
    void HandleMoves(int PlayerColor);
    //Display the Board on GUI
    void DisplayBoard() const;

    //Reset or Restart a Game
    void ResetBoard(bool calculateELO);
    void RestartGame();

    //Return Back to Menu
    void BackToMenu();

    //Calculate Player ELO's
    void UpdateElos();

    //FEN related Functions
    void setFENstring(std::string newFen);
    void FENSettings();

    void SaveTranspositions();
    void BoardSetUp();
    void Settings();
    void Destroy();

    void SetFENStrings(const std::string& fen);

    void GameLoop();

};



#endif // !GAMEMODES_H





