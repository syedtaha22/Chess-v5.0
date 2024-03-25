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

//int main(){
//    InitWindow(screenWidth, screenHeight, "Chess");
//    InitAudioDevice();
//
//    cout << "\n\n\n\n\n";
//
//    Flags flags;
//    GameModes Game;
//    Menu GameMenu;
//
//    Game.Horizon.SetDepth(10);
//    ChessBoard temp;
//    //temp.initializeBoardFromFEN("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq");
//
//
//
//    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
//
//    //Game.setFENstring(FENString);
//    //Game.BoardSetUp();
//
//
//    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
//    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
//    KingChecked = LoadSound(KingCheckedFile.c_str());
//    GameStarts = LoadSound(GameStartsFile.c_str());
//    GameEnds = LoadSound(GameEndsFile.c_str());
//    myFont = LoadFont(fontFile.c_str());    
//    Image icon = LoadImage(program_icon.c_str());
//
//    SetWindowIcon(icon);
//    SetTargetFPS(60);
//
//    string fens[70] = {
//        "1k6/ppp3p1/8/1P5p/8/P3n2P/2P1r1P1/B2rNRK1 w",
//        "2b2r1k/pp4p1/2p5/4r3/2B5/1P6/P4pK1/5R2 w",
//        "1r6/5p1R/7R/4pPk1/2r2n2/1P3P1P/P4K2/8 w",
//        "8/2k4p/2P1K1p1/5p2/5P2/8/7P/8 w",
//        "r3r1k1/pp5p/2pq2p1/3nNQ1b/3P4/4pP1P/PP2P1B1/1K1R3R w",
//        "r1b2rk1/ppp2ppp/3p4/3q4/P7/3BP2P/1PPQ1PP1/1R2K2R w K",
//        "r3k2r/pp3ppp/1bp2n2/4p3/Q3Pqb1/B1PB4/P4PPP/R3NRK1 w kq",
//        "3r1rk1/p4p1p/4nQp1/3pp1P1/1P5P/2N1P3/2PK1Pq1/1R5R w",
//        "r4rk1/ppp3pp/2nb2b1/5p2/2P1p1Pq/1B2P2P/PB1P1PK1/R2QNR2 w",
//        "8/8/1p1r2k1/pR1P1pp1/P2K2P1/8/8/8 w",
//        "1k2r3/p2p4/1p1q2p1/3br1B1/Q6P/1P4P1/P7/R3R1K1 w",
//        "r2q1r1k/1bpn2b1/pp4Q1/5p2/2BPp3/P6P/1PP2PPN/R4RK1 w",
//        "1r3rk1/pq3ppp/n2b1p2/8/Q1ppPP1P/P2P2P1/3P2B1/R1N2RK1 w",
//        "6r1/4k3/R3P3/p2p1R2/P2Pn1r1/6P1/6K1/8 w",
//        "3b4/8/1pP1k3/P7/6Kp/7P/8/8 w",
//        "r2q1rk1/pp3ppp/2P1p3/5b2/2P2B2/6P1/P4PBP/b2Q1RK1 w",
//        "8/pp3k2/1npQ4/3pP3/8/2P1b3/PP5q/1K1b1B2 w",
//        "8/2N1b3/1k3p2/4pPp1/2p1P3/5P2/1PK5/8 w",
//        "6k1/4bppp/qpQ5/2p1N3/3pP3/7P/1r3PPK/r7 w",
//        "8/3k4/6pp/1P3p2/4p3/5bP1/5P1P/4K1N1 w",
//        "5k2/pb4bQ/1pr1p1pq/n2pP3/8/P3R1PB/1P3P1P/5NK1 w",
//        "8/5kp1/1N3p1p/1p2p3/2b1P3/5PP1/rR4KP/8 w",
//        "8/8/6RP/8/1p1kr3/4p3/4K3/8 w",
//        "6k1/2r2n1p/p5p1/1pNP1pP1/8/1P6/1P4P1/4R1K1 w",
//        "r2q3r/1p2b1k1/2n2p2/2p2PpQ/4P3/P1pP4/2P3B1/R3K1NR w KQ",
//        "5rk1/3Q1ppp/8/p1bB4/8/4P2P/3q1PKP/6R1 w",
//        "6k1/2p4p/1bn3p1/3p1nPb/2p1p2q/2Q1P3/1B3PBN/5NK1 w",
//        "2rqr1k1/pp3ppp/8/3n2N1/3P2PP/3Q4/PP3p2/1K1R3R w",
//        "1r4k1/5ppp/1Pp2q2/2P2b2/R3p3/4P3/5PPP/3Q2K1 w",
//        "8/k1pr4/1p6/4R3/p2r2p1/q5P1/5P2/1R4KQ w",
//        "r3k2r/p1p2p2/PpPp4/1P1P3n/3QPpb1/1N6/1B3KBq/R7 w kq",
//        "r3r1k1/1p3ppp/2pp1q1n/7P/8/p4PQ1/PPPB1P2/2K3RR w",
//        "r2qk2r/1pp2ppp/p1n5/5nPb/Nb1p1B1P/3P4/PPPQP1B1/2KR2NR w kq",
//        "2krRr2/pp1n3p/2p3p1/3p2Q1/3q2P1/2NP4/PP5P/2K1R3 w",
//        "5r1k/p4r2/2pp2Q1/2p5/P2qP3/1P1P4/2P4P/6RK w",
//        "7k/pp4r1/8/8/4r1qb/4QR1p/PP2R3/5B1K w",
//        "r3r1k1/1ppqbppp/p1n1b3/3n4/1P6/P1Q1PN2/1B1NBPPP/R3K2R w KQ",
//        "r4rk1/pp3p1p/6q1/2p4Q/2Pb1R2/3P2P1/PP3P1P/R5K1 w",
//        "Rnk3r1/2pn3p/8/1N2p3/8/4P2r/5RB1/6K1 w",
//        "5nk1/ppq4p/2p3p1/3p1NP1/4rb1P/PP2P3/1BP3K1/3RQ3 w",
//        "2b4k/2p5/2p1r2q/p2p3Q/8/2P3P1/P1P4P/4R2K w",
//        "6rr/1k6/2n5/1p3P2/3pP2p/1QPP1q2/1P4PP/R4NK1 w",
//        "6k1/5rb1/2p3np/1pBp1p2/3Pp3/2N1PpPq/2P2P1P/Q5RK w",
//        "r6k/7p/2p3pQ/1p2p3/1P1pP1q1/3P4/p1P5/K4R2 w",
//        "1r1r2k1/R4pp1/4b2p/2Q1R2P/1ppP1P2/2P3P1/6BK/3q4 w",
//        "5rk1/1p3p1p/1q2pp2/nN1p4/R7/4P3/1Q1P1PPP/6K1 w",
//        "rn2k2r/1p2pp2/p1p4p/4b2P/2P1Nqp1/P7/1P2Q1P1/1K1R2NR w kq",
//        "rn1qk2r/ppp2ppp/3b1n2/3pp2N/6b1/3PP3/PPP2PPP/RNBQKB1R w KQkq",
//        "k2r4/ppQ2p2/2r5/1N1p4/1K1Pq3/P3P1P1/1P6/8 w",
//        "rn1q1rk1/1pp1bpp1/4bn1p/1P1pp3/7N/B1PP2P1/2Q1PPBP/RN2K2R w KQ",
//        "2kr3r/ppp1bpp1/2q2n2/1N1p3p/3R4/4P1Q1/PPP2PPP/R1B3K1 w",
//        "1r4k1/1P1b3p/5p2/3P1Pp1/1Br1p1Pb/2N4P/6K1/1R6 w",
//        "r2n2k1/6p1/2p1p1Qp/2Pn4/3q4/3B2P1/3B3P/4R1K1 w",
//        "8/8/4k1p1/5p1p/5K1P/5PP1/8/8 w",
//        "3r1b1r/ppk3pp/2n2n2/4qb2/1PB5/P1N1PP2/1B1PQ2P/R3K2R w KQ",
//        "rn1qkbnr/pp3ppp/4p3/2pp4/6b1/1QP5/PP1PPPBP/RNB1K1NR w KQkq",
//        "r4r1k/pp4pp/3q2p1/b7/2BB4/P3P1P1/4KPP1/R6R w",
//        "5r1k/p1b3pp/1p6/1PpQp1q1/2Pp1n2/P2P2P1/2N1R1K1/5R2 w",
//        "1k6/pb6/1p2Np2/8/1b5p/1P2B2P/P4Pr1/2R4K w",
//        "6k1/5p1p/7B/1R2p3/3p4/5qP1/2r4P/6K1 w",
//        "2r5/p2nkppp/1p2p3/8/5P2/2r1N1P1/P6P/R3R1K1 w",
//        "r4r2/pRp2kp1/8/4q3/3n4/5P2/b1P1N1P1/3QK2R w",
//        "8/5p2/1k5p/1P2N1p1/K3P3/4r3/8/8 w",
//        "r2qk2r/ppp2ppp/2n2n2/4N3/Q3P1b1/2bP2P1/PP3PBP/R1B1K2R w KQkq",
//        "2r1kb1r/1p1b1ppp/1q2p3/pn6/P2P4/1nN1PN2/1P3P1P/1RBQK2R w Kk",
//        "R7/1r3p2/4p3/r7/1k4P1/p3RPK1/8/8 w",
//        "r1r3k1/p2qbppp/1p2pn2/8/3P4/4PQP1/1BnNBP1P/R1R3K1 w",
//        "r3r1k1/1pp2ppp/p1n3q1/3p1bn1/3P4/P1BBPQ1P/1P3PP1/RN3RK1 w",
//        "8/2B2p2/p1n3p1/2ppP1P1/5k2/P2K4/1PP5/8 w",
//        "6k1/2p2ppp/4r3/Rp6/8/P3P3/5PPP/b5K1 w"
//    };
//
//    thread aiThread(CalculateAIMove, ref(Game.Horizon), ref(Game.chessboard));
//    aiThread.detach();
//    int index = 0;
//    // Main menu loop
//    while (!WindowShouldClose()) {
//
//        // Draw
//        BeginDrawing();
//        ClearBackground(Background);
//        GameMenu.DrawMenuBox();
//        
//        // Check if the game has started
//        if (flags.isGameStarted()) {
//
//            if (flags.isSinglePlayer()) {
//                Game.SinglePlayerMode();
//            }
//            else if (flags.isMultiplayerGame()) {
//                Game.MultiplayerMode();
//
//            }
//        }
//
//        else if (flags.isFENSettingsOpened()) Game.FENSettings();
//        else if (flags.SettingsOpened()) Game.Settings();
//
//        else {
//
//            if (GameMenu.StartSingleplayer || GameMenu.StartMultiplayer) {
//                flags.StartGame();
//                if (GameMenu.StartSingleplayer) {
//                    flags.SinglePlayerMode();
//                }
//                else if (GameMenu.StartMultiplayer) {
//                    flags.MultiplayerMode();
//                }
//            }
//            if (GameMenu.OpenSettings) {
//                flags.OpenSettings();
//            }
//            if (GameMenu.LoadFromFen) {
//                flags.OpenFENSettings();
//            }
//
//            Game.DisplayBoard();
//            GameMenu.ShowMenu();
//
//        }
//        //temp.initializeBoardFromFEN(fens[index]);
//        if (index == 69) index = 0;
//        else index++;
//        //temp.DrawChessPiece();
//
//        EndDrawing();
//    }
//    //Game.SaveTranspositions();
//    Game.chessboard.saveCurrentFENtofile("UnexpectedExits.txt");
//    Game.Destroy();
//    
//    UnloadSound(ChessPiecePlaced);
//    UnloadSound(ChessPieceCaptured);
//    UnloadSound(KingChecked);
//    UnloadSound(GameEnds);
//    UnloadSound(GameStarts);
//    UnloadFont(myFont);
//    UnloadImage(icon);
//    CloseAudioDevice();
//    CloseWindow();
//    return 0;
//}

//int main(){
//    InitWindow(screenWidth, screenHeight, "Chess");
//    InitAudioDevice();
//
//    cout << "\n\n\n\n\n";
//
//    ChessBoard chessboard;
//
//    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
//
//    chessboard.initializeBoard();
//
//
//
//    SetTargetFPS(60);
//
//
//    while (!WindowShouldClose()) {
//
//        // Draw
//        BeginDrawing();
//        ClearBackground(Background);
//        chessboard.DrawBoard();
//        chessboard.DrawChessPiece();
//        for (int i = 0; i < Total_tiles; i++) {
//            chessboard.UpdateChessPiece(chessboard.board[i], i);
//        }
//
//
//        EndDrawing();
//    }
//
//    
//    UnloadSound(ChessPiecePlaced);
//    UnloadSound(ChessPieceCaptured);
//    UnloadSound(KingChecked);
//    UnloadSound(GameEnds);
//    UnloadSound(GameStarts);
//    UnloadFont(myFont);
//
//    CloseAudioDevice();
//    CloseWindow();
//    return 0;
//}

int main() {
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();

    cout << "\n\n\n\n\n";

    Flags flags;
    GameModes Game;
    Menu GameMenu;

    Game.Horizon.SetDepth(2);
    ChessBoard temp;
    //temp.initializeBoardFromFEN("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq");



    //string FENString = "7k/P7/K7/8/8/8/PPPPPPPP/RNR4 w KQkq"; //--->White Pawn Promotion
    //string FENString = "8/8/8/8/8/k7/p7/7K w KQkq"; //--->Black Pawn Promotion

    //Game.setFENstring(FENString);
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

    thread aiThread(CalculateAIMove, ref(Game.Horizon), ref(Game.chessboard));
    aiThread.detach();
    int index = 0;
    // Main menu loop
    while (!WindowShouldClose()) {

        // Draw
        BeginDrawing();
        ClearBackground(Background);
        GameMenu.DrawMenuBox();

        // Check if the game has started
        if (flags.isGameStarted()) {

            if (flags.isSinglePlayer()) {
                Game.SinglePlayerMode();
            }
            else if (flags.isMultiplayerGame()) {
                Game.MultiplayerMode();

            }
        }

        else if (flags.isFENSettingsOpened()) Game.FENSettings();
        else if (flags.SettingsOpened()) Game.Settings();

        else {

            if (GameMenu.StartSingleplayer || GameMenu.StartMultiplayer) {
                flags.StartGame();
                if (GameMenu.StartSingleplayer) {
                    flags.SinglePlayerMode();
                }
                else if (GameMenu.StartMultiplayer) {
                    flags.MultiplayerMode();
                }
            }
            if (GameMenu.OpenSettings) {
                flags.OpenSettings();
            }
            if (GameMenu.LoadFromFen) {
                flags.OpenFENSettings();
            }

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

//int main() {
//    InitWindow(screenWidth, screenHeight, "Chess");
//    InitAudioDevice();
//
//    cout << "\n\n\n\n\n";
//
//    Flags flags;
//    GameModes Game;
//    Menu GameMenu;
//    GraphicalBoard board;
//
//    Game.Horizon.SetDepth(1);
//    ChessBoard temp;
//    //temp.initializeBoardFromFEN("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq");
//
//
//
//    //string FENString = "7k/P7/K7/8/8/8/PPPPPPPP/RNR4 w KQkq"; //--->White Pawn Promotion
//    string FENString = "8/8/8/8/8/k7/p7/7K w KQkq"; //--->Black Pawn Promotion
//
//    Game.setFENstring(FENString);
//    Game.BoardSetUp();
//
//
//    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
//    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
//    KingChecked = LoadSound(KingCheckedFile.c_str());
//    GameStarts = LoadSound(GameStartsFile.c_str());
//    GameEnds = LoadSound(GameEndsFile.c_str());
//    myFont = LoadFont(fontFile.c_str());
//    Image icon = LoadImage(program_icon.c_str());
//
//    SetWindowIcon(icon);
//    SetTargetFPS(60);
//
//    thread aiThread(CalculateAIMove, ref(Game.Horizon), ref(Game.chessboard));
//    aiThread.detach();
//    int index = 0;
//    // Main menu loop
//    while (!WindowShouldClose()) {
//
//        // Draw
//        BeginDrawing();
//        ClearBackground(Background);
//        board.DrawBoard(Game.chessboard);
//
//
//
//        EndDrawing();
//    }
//    //Game.SaveTranspositions();
//    Game.chessboard.saveCurrentFENtofile("UnexpectedExits.txt");
//    Game.Destroy();
//
//    UnloadSound(ChessPiecePlaced);
//    UnloadSound(ChessPieceCaptured);
//    UnloadSound(KingChecked);
//    UnloadSound(GameEnds);
//    UnloadSound(GameStarts);
//    UnloadFont(myFont);
//    UnloadImage(icon);
//    CloseAudioDevice();
//    CloseWindow();
//    return 0;
//}

