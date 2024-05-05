#include "../../headers/Other/GameModes.h"


#include "raygui.h" 

GameModes::GameModes() {
    DoOnce = true;

    //Default FEN
    FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
}

void GameModes::InitialiseMultiplayerMode() {
    if (DoOnce) {
        PlaySound(GameStarts);
        chessboard.initializeBoardFromFEN(FENString, true);
        chessboard.ComputeOpponentMoves();
        DoOnce = false;
        Flags::DisableSinglePlayer();
    }
}

void GameModes::Options() {
    if (IsKeyPressed(KEY_H)) {
        GameStats.ShowMoveHistory = !GameStats.ShowMoveHistory;
    }
    if (IsKeyPressed(KEY_R)) RestartGame();
    if (IsKeyPressed(KEY_M)) BackToMenu();
}

void GameModes::HandleMoves(int PlayerColor) {
    for (int i = 0; i < Total_tiles; i++) {
        if (chessboard.board[i]->color == PlayerColor) {
            chessboard.UpdateChessPiece(chessboard.board[i], i);
        }
    }
}

void GameModes::MultiplayerMode() {
    InitialiseMultiplayerMode();
    if (!GameStats.GameIsEnded(chessboard)) {
        if (chessboard.isCurrentPlayerWhite()) {
            HandleMoves(White);
        }
        else {
            HandleMoves(Black);
        }
        Options();
        if (GameStats.ShowMoveHistory) GameStats.DisplayMoveHistory(chessboard.getMoveHistory());
        else GameStats.DisplayStats(chessboard);
        DisplayBoard();

    }
    else {
        ResetBoard(false);
    }
}

void GameModes::ResetBoard(bool calculateELO) {
    DisplayBoard();

    GameStats.DisplayEndMessage();
    Horizon.TerminateSearch();

    if (GameStats.SaveData) {
        if (calculateELO) CalculateELO();
        chessboard.saveCurrentFENtofile("MatchHistory.txt");
        GameStats.SaveData = false;
    }
    if (IsKeyPressed(KEY_R)) RestartGame();
    if (IsKeyPressed(KEY_M)) BackToMenu();
}

void GameModes::RestartGame() {

    Horizon.TerminateSearch();
    DoOnce = true;
    GameStats.Reset();
    Horizon.Reset();
    chessboard.DestroyBoard();
}

void GameModes::BackToMenu() {
    DoOnce = true;
    GameStats.Reset();
    Horizon.Reset();
    chessboard.DestroyBoard();
    chessboard.initializeBoardFromFEN(FENString, true);
    Flags::EndGame();
}

void GameModes::CalculateELO() {
    int oldBlackELO = Horizon.engineEloRating;
    Horizon.engineEloRating = GameStats.updateEloRating(Horizon.engineEloRating, Player.ELO, (GameStats.winner == Black));
    Player.ELO = GameStats.updateEloRating(Player.ELO, oldBlackELO, (GameStats.winner == White));
    Settings::save(Horizon.getDepth(), Player.ELO, Horizon.engineEloRating);

}

void GameModes::InitialiseSinglePlayerMode() {
    if (DoOnce) {
        Horizon.setEngineColor(Black);

        Player.setUserName("user");
        PlaySound(GameStarts);
        //chessboard.InitializeDefaultBoard();
        chessboard.ComputeOpponentMoves();


        DoOnce = false;
        //isSinglePlayer = true;
    }

}

void GameModes::SinglePlayerMode() {
    InitialiseSinglePlayerMode();
    if (!GameStats.GameIsEnded(chessboard)) {

        if (chessboard.isCurrentPlayerWhite()) HandleMoves(White);
        else Horizon.StartSearch();


        Options();
        if (GameStats.ShowMoveHistory) GameStats.DisplayMoveHistory(chessboard.getMoveHistory());
        else GameStats.DisplayStats(chessboard, Horizon, Player);
        DisplayBoard();
        GameStats.DrawEvaluationColumn(chessboard, Horizon);
    }
    else ResetBoard(true);
}

void GameModes::DisplayBoard() const {
    graphics.DrawBoard(chessboard);
    graphics.DrawChessPiece(chessboard);
    //chessboard.DrawSquareIndices();
    graphics.DrawCoordinates(chessboard);
}

void GameModes::Destroy() {
    chessboard.DestroyBoard();
}

void GameModes::setFENstring(string newFen) {
    FENString = newFen;
}

void GameModes::SaveTranspositions() {
    Horizon.SaveTranspositionTable();
}
  
void GameModes::BoardSetUp() {
    chessboard.initializeBoardFromFEN(FENString, true);
    //chessboard.InitializeDefaultBoard();
    //chessboard.initializeBoard();
}

void GameModes::Settings() {
    Rectangle SetDepthBox = { (InfoBoxWidth / 2.0f) - 40.0f, screenHeight / 2.0f - 20.0f, 200.0f, 40.0f };
    GuiTextBox(SetDepthBox, inputDepth, 3, true);
    enteredDepth = atoi(inputDepth);

    GameStats.DisplayNewDepthMessage(enteredDepth);
    if (IsKeyPressed(KEY_ENTER)) {
        Horizon.SetDepth(enteredDepth);
        Settings::saveElement(Settings::depth, enteredDepth);
        Flags::closeSettings();
    }
}

void GameModes::FENSettings() {
    Rectangle SetFENBox = { (InfoBoxWidth / 2.0f) - 40.0f, screenHeight / 2.0f - 20.0f, 200.0f, 40.0f };
    GuiTextBox(SetFENBox, feninput, 76, true);
   
    string fen = feninput;
    GameStats.DisplayNewFENMessage(fen);
    if (IsKeyPressed(KEY_ENTER)) {
        if(fen != "") setFENstring(fen);
        Flags::closeFENSettings();
    }

}

void GameModes::SetFENStrings(const string& fen) {
    FENString = fen;
    chessboard.DestroyBoard();
    chessboard.initializeBoardFromFEN(FENString, true);
}


