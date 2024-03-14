#ifndef FLAGS_H
#define FLAGS_H

class Flags {

    static bool isMoveMade;
    static bool SinglePlayer;
    static bool MultiplayerGame;
    static bool gameStarted;
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

};

bool Flags::isMoveMade = false;
bool Flags::SinglePlayer = false;
bool Flags::MultiplayerGame = false;
bool Flags::gameStarted = false;


#endif // !FLAGS_H



