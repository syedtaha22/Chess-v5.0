#include "headers/Other/GameModes.h"
#include "headers/Other/Resources.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"


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




int main(){
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();

    Flags flags;

    GameModes Game;



    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    //string FENString = "rnbqkbnr/qqqqqqqq/qqqqqqqq/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    //string FENString = "1rb3k1/p2q2bp/2p4r/2P1p1p1/3pPp2/1P1P1P2/PB1NN1KP/1R2QR2 b";
    //string FENString = "qqqqkqqq/qqqqqqqq/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    // string FENString = "2k5/ppp5/8/8/8/8/2P1P1PP/4K3 w KQkq";
    // string FENString = "8/p5k1/8/8/8/8/7P/7K w KQkq";
    //  string FENString = "3rr3/3k4/8/8/3K4/8/8/8 w KQkq";
     //string FENString = "4kbnr/8/8/8/8/8/1p6/4K2R w Kk";
    // string FENString = "6Q1/8/8/8/8/8/K5R1/7k w KQkq";
    // chessboard.initializeBoard();
    //chessboard.initializeBoardFromFEN(FENString);

    Game.setFENstring(FENString);
    Game.BoardSetUp();


    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
    KingChecked = LoadSound(KingCheckedFile.c_str());
    GameStarts = LoadSound(GameStartsFile.c_str());
    GameEnds = LoadSound(GameEndsFile.c_str());

    Image icon = LoadImage(program_icon.c_str());

    SetWindowIcon(icon);

    myFont = LoadFont(fontFile.c_str());
    string move;

    //bool DoneOnce = false;
    SetTargetFPS(60);
    //bool gameStarted = false;
    bool StartGame = false;
    bool StartMultiplayer = false;

    Rectangle InfoBox = { InfoBoxX, InfoBoxY, InfoBoxWidth, InfoBoxHeight };
    Rectangle StartButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 3) - buttonCenterY, 200, 70 };
    Rectangle MultiplayerButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 2) - buttonCenterY, 200, 70 };
    Rectangle FENButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 1.5f) - buttonCenterY, 200, 70 };

    thread aiThread(CalculateAIMove, ref(Game.Horizon), ref(Game.chessboard));
    aiThread.detach(); // Detach the thread to let it run independentl

    // Main menu loop
    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
        ClearBackground(Background);
        DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent);
        // Check if the game has started
        if (flags.isGameStarted()) {

            if (flags.isSinglePlayer()) {
                Game.SinglePlayerMode();
            }
            else if (flags.isMultiplayerGame()) {
                Game.MultiplayerMode();

            }
        }

        else {

            if (StartGame || StartMultiplayer) {
                flags.StartGame();
                if (StartGame) {
                    flags.SinglePlayerMode();
                }
                else if (StartMultiplayer) {
                    flags.MultiplayerMode();
                }
            }

            Game.DisplayBoard();


            StartGame = GuiButton(StartButton, "Start");
            StartMultiplayer = GuiButton(MultiplayerButton, "Multiplayer");

        }
        EndDrawing();
    }
    Game.SaveTranspositions();
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