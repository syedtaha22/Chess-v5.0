#ifndef FLAGS_H
#define FLAGS_H

class Flags {
private:
    static bool isMoveMade;
    static bool SinglePlayer;
    static bool MultiplayerGame;
    static bool gameStarted;
    static bool showSettings;
    static bool SetFEN;

public:

    Flags() {}

    static void CheckGameState();
    static void GameStateChecked();
    static bool MoveIsMade();
    static void StartGame();
    static bool isMultiplayerGame();
    static bool isSinglePlayer();
    static bool isGameStarted();
    static void EndGame();
    static void SinglePlayerMode();
    static void MultiplayerMode();
    static void DisableSinglePlayer();
    static void OpenSettings();
    static bool SettingsOpened();
    static void closeSettings();
    static void OpenFENSettings();
    static bool isFENSettingsOpened();
    static void closeFENSettings();


};



#endif // !FLAGS_H



