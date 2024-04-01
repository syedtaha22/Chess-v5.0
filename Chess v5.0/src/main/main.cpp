#include "../../headers/Other/GameModes.h"
#include "../../headers/Other/Resources.h"
//#include "../../headers/Other/Menu.h"

void CalculateAIMove(ChessEngine& engine, ChessBoard& board) {
    while (true) {
        if (engine.isSearchStarted() && !board.isCurrentPlayerWhite()) {

            //string move;
            string move;
            board.ComputeOpponentMoves();
            cout << "Running...\n";
            move = engine.GenerateMove(board);
            if (move != "") {
                engine.PlayMove(move, board);
            }
            engine.StopSearching();

        }

    }
}



int main() {
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();


    GameModes Game;
    Menu GameMenu;

    Game.Horizon.SetDepth(2);
    Game.BoardSetUp();

    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
    KingChecked = LoadSound(KingCheckedFile.c_str());
    GameStarts = LoadSound(GameStartsFile.c_str());
    GameEnds = LoadSound(GameEndsFile.c_str());
    myFont = LoadFont(fontFile.c_str());
    Image icon = LoadImage(program_icon.c_str());

    SetWindowIcon(icon);
    SetTargetFPS(60);

    thread EngineThread(CalculateAIMove, ref(Game.Horizon), ref(Game.chessboard));
    EngineThread.detach();
    int index = 0;
    // Main menu loop
    while (!WindowShouldClose()) {

        // Draw
        BeginDrawing();
        ClearBackground(Background);
        GameMenu.DrawMenuBox();

        // Check if the game has started
        if (Flags::isGameStarted()) {
            if (Flags::isSinglePlayer()) Game.SinglePlayerMode();
            else if (Flags::isMultiplayerGame()) Game.MultiplayerMode();
        }
        else if (Flags::isFENSettingsOpened()) Game.FENSettings();
        else if (Flags::SettingsOpened()) Game.Settings();
        else {
            if (GameMenu.StartSingleplayer || GameMenu.StartMultiplayer) {
                Flags::StartGame();
                if (GameMenu.StartSingleplayer) Flags::SinglePlayerMode();
                else if (GameMenu.StartMultiplayer) Flags::MultiplayerMode();
            }
            if (GameMenu.OpenSettings) Flags::OpenSettings();
            if (GameMenu.LoadFromFen) Flags::OpenFENSettings();
            Game.DisplayBoard();
            GameMenu.ShowMenu();
        }
        EndDrawing();
    }
    //Game.SaveTranspositions();
    Game.chessboard.saveCurrentFENtofile("UnexpectedExits.txt");
    Game.Destroy();

    UnloadSound(ChessPiecePlaced);
    UnloadSound(ChessPieceCaptured);
    UnloadSound(KingChecked);
    UnloadSound(GameEnds);
    UnloadSound(GameStarts);
    UnloadFont(myFont);
    UnloadImage(icon);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}