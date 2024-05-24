#include "../../headers/GameFlow/GameModes.h"
#include "../../headers/Other/Resources.h"
//#include "../../headers/Other/Menu.h"


int main() {
    //Supresses the Ralib Log Messages
    SetTraceLogCallback(Utility::SuppressRaylibLog);

    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();

    GameModes Game;
    Game.BoardSetUp();
    
    Settings::save(Game.Horizon.state.getDepth(), Game.Player.ELO, Game.Horizon.state.getELO());

    Utility::LoadFontsAndSounds();

    Image icon = LoadImage(program_icon.c_str());
    SetWindowIcon(icon);


    SetTargetFPS(60);

    //Activate Engine Thread
    std::thread EngineThread(Utility::Engine_Calculations, std::ref(Game.Horizon), std::ref(Game.chessboard));
    EngineThread.detach(); // Allow it to Run independently

    // Main menu loop
    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(Background);
        Game.GameLoop();
        EndDrawing();
    }
    //Game.SaveTranspositions();
    Game.chessboard.saveCurrentFENtofile("UnexpectedExits.txt");
    Game.chessboard.DestroyBoard();

    Utility::UnloadFontsAndSounds();
    UnloadImage(icon);


    CloseAudioDevice();
    CloseWindow();
    return 0;
}