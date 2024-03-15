#include "../../headers/Other/BoardStats.h"



BoardStats::BoardStats() {
    CheckAlertB = messageColor;
    CheckAlertW = messageColor;
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
    EndMessage = "";
}

void BoardStats::DisplayEndMessage() {
    float posX = textX - ((TextCenter(EndMessage.c_str(), fontSize - 10).x) / 2);
    float posY = (static_cast<float>(screenHeight) / 2) - ((TextCenter(EndMessage.c_str(), fontSize - 10).y) / 2);

    DrawTextWithCustomFont(EndMessage.c_str(), posX, posY, fontSize - 10, AlertColor); // Balance Based on TextSize
}

bool BoardStats::GameIsEnded(ChessBoard& board) {

    if (flags.MoveIsMade()) {

        cout << "Checking....." << endl;
        int playerColor = board.isCurrentPlayerWhite() ? White : Black;

        if (board.isCheckmate(board, playerColor)) {

            //Set message to display
            EndMessage = "Winner is ";
            EndMessage += (board.isCurrentPlayerWhite() ? Black_ : White_);
            winner = board.isCurrentPlayerWhite() ? Black : White; // Set Winner

            flags.GameStateChecked();
            cout << "Check Complete" << endl;
            return true;
        }
        if (!board.isCheck(board, playerColor, "stats: Game is Ended: stalemate") && board.GetAllPossibleMoves(playerColor).size() == 0) {
            EndMessage = "---Stalemate---";
            winner = -1; //No Winner
            flags.GameStateChecked();
            cout << "Check Complete" << endl;
            return true;
        }
        if (board.calculatePlayerScore(White) <= board.getPieceScore(KING) && board.calculatePlayerScore(Black) <= board.getPieceScore(KING)) {
            EndMessage = "---Draw---";
            winner = -1; //No Winner
            flags.GameStateChecked();
            return true;
            cout << "Check Complete" << endl;
        }

        cout << "Check Complete" << endl;
        flags.GameStateChecked();
    }

    if (winner != NULL) return true;
    else return false;

}

string BoardStats::CloseInBrackets(string stringClose) {
    string temp = "(";
    temp += stringClose;
    temp += ")";
    return temp;
}

void BoardStats::DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player) {
   


    string LastMovePlayed = "";
    string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

    if (flags.MoveIsMade()) {
        cout << "Entered Here" << endl;
        //cout << flags.MoveIsMade << endl;
        
        Color CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? AlertColor : messageColor;
        Color CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? AlertColor : messageColor;

    }

    if (chessboard.moveHistory.size() != 0) {
        LastMovePlayed = (chessboard.moveHistory.back());
    }




    MovesAndHistory(LastMovePlayed, chessboard);
    DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
    DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertW);
    // (isSinglePlayer) {
    int LookAheads = engine.NumberofMovesLookedAhead;
    int BranchesPruned = engine.NumberOfBranchesPruned;
    int FoundTranspostions = engine.NumberOfTranspositionsFound;
    long long TimeTaken = engine.TimeTakenForSearch;
    double SizeOfTable = engine.getSizeOfTranspositionTable();

    string WhiteELO = CloseInBrackets(ELO + to_string(player.ELO));
    string BLackELO = CloseInBrackets(ELO + to_string(engine.engineEloRating));
    string BlackMessage = "Saw " + to_string(LookAheads) + " futures in " + to_string(TimeTaken) + "s";
    string PruningMessage = "Pruned " + to_string(BranchesPruned) + " Branches";
    string TableSizeMessage = "Size: " + to_string(SizeOfTable);
    string TranspostionsFound = "Total Transpostions Found: " + to_string(FoundTranspostions);
    DrawTextWithCustomFont(BlackMessage.c_str(), textX - (TextCenter(BlackMessage.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
    DrawTextWithCustomFont(TableSizeMessage.c_str(), textX - (TextCenter(TableSizeMessage.c_str(), fontSize - 30).x / 2), textY + 90, fontSize - 30, messageColor);
    DrawTextWithCustomFont(PruningMessage.c_str(), textX - (TextCenter(PruningMessage.c_str(), fontSize - 30).x / 2), textY + 110, fontSize - 30, messageColor);
    DrawTextWithCustomFont(TranspostionsFound.c_str(), textX - (TextCenter(TranspostionsFound.c_str(), fontSize - 30).x / 2), textY + 130, fontSize - 30, messageColor);

    DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), screenHeight - 146 + 30, fontSize - 30, messageColor);
    DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);
    //}

    // if (!chessboard.preMoves.isEmpty()) {
    //     string NextMoveToPlay = "Next in Queue: " + chessboard.preMoves.peek();
    //     DrawTextWithCustomFont(NextMoveToPlay.c_str(), textX - strlen(White_) - 20, screenHeight - 166 - fontSize + 30, fontSize - 30, messageColor);
    // }
}

void BoardStats::DisplayStats(ChessBoard& chessboard) {
    Color CheckAlertB = messageColor;
    Color CheckAlertW = messageColor;
    string LastMovePlayed = "";
    string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

    if (flags.MoveIsMade()) {
        //cout << flags.MoveIsMade << endl;
        CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? messageColor : AlertColor;
        CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? messageColor : AlertColor;
    }

    if (chessboard.moveHistory.size() != 0) {
        LastMovePlayed = (chessboard.moveHistory.back());
    }

    MovesAndHistory(LastMovePlayed, chessboard);
    DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
    DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertB);
}

void BoardStats::MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard) {
    if (ShowMoveHistory) {
        DisplayMoveHistory(chessboard);
    }
    else {
        DrawTextWithCustomFont(LastMovePlayed.c_str(), textX - (TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2), screenHeight / 2 - 42, fontSize - 10, AlertColor);
    }
}

// void DisplayStats(ChessBoard chessboard, ChessEngine Black, ChessEngine White) {
//     int LookAheadsBlack = Black.NumberofMovesLookedAhead;
//     auto TimeTakenBlack = Black.TimeTakenForSearch;
//     int LookAheadsWhite = White.NumberofMovesLookedAhead;
//     auto TimeTakenWhite = White.TimeTakenForSearch;
//     int whiteScore = chessboard.calculatePlayerScore('W');
//     int blackScore = chessboard.calculatePlayerScore('B');
//     string LastMovePlayed = "";
//     string BlackMessage = "Saw " + to_string(LookAheadsBlack) + " futures in " + to_string(TimeTakenBlack) + " milliseconds";
//     string WhiteMessage = "Saw " + to_string(LookAheadsWhite) + " futures in " + to_string(TimeTakenWhite) + " milliseconds";
//     string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
//     if (chessboard.moveHistory.size() != 0) {
//         LastMovePlayed = chessboard.moveHistory.back();
//     }
//     //Display if Black Is in check
//     //Display Black Score
//     //Display Engine message
//     //Display Last Move
//     //Display Current Player
//     //Display white Score
//     //Display if White Is in Check
//     //Black
//     if (chessboard.isCheck(chessboard, 'B', "Stats: DisplayStats: black: 2")) {
//         DrawTextWithCustomFont("Black King is in Check", textX, (tileSize * 1) - fontSize, fontSize, RED);
//     }
//     else {
//         DrawTextWithCustomFont("Black King is not in Check", textX, (tileSize * 1) - fontSize, fontSize, BLACK);
//     }
//     DrawTextWithCustomFont(("Black ELO: " + to_string(Black.engineEloRating)).c_str(), textX, (tileSize * 2 - fontSize) - fontSize, fontSize, BLACK);
//     DrawTextWithCustomFont(("Black Player Score: " + to_string(blackScore)).c_str(), textX, (tileSize * 3 - fontSize) - fontSize, fontSize, BLACK);
//     DrawTextWithCustomFont(BlackMessage.c_str(), textX, (tileSize * 2 + fontSize) - (fontSize - 0), fontSize - 4, BLACK);
//     //Current Player
//     DrawTextWithCustomFont(("Current Player: " + currentPlayer).c_str(), textX, static_cast<float>(screenHeight) / 2, fontSize, (currentPlayer == "White") ? WHITE : BLACK);
//     DrawTextWithCustomFont(LastMovePlayed.c_str(), textX, (tileSize * 5 + fontSize) - fontSize, fontSize, RED);
//     //White
//     DrawTextWithCustomFont(WhiteMessage.c_str(), textX, (tileSize * 7 + fontSize) - (fontSize - 0), fontSize - 4, WHITE);
//     DrawTextWithCustomFont(("White Player Score: " + to_string(whiteScore)).c_str(), textX, (tileSize * 5 + fontSize) + fontSize, fontSize, WHITE);
//     DrawTextWithCustomFont(("White ELO: " + to_string(White.engineEloRating)).c_str(), textX, (tileSize * 6 + fontSize) + fontSize, fontSize, WHITE);
//     if (chessboard.isCheck(chessboard, 'W', "Stats: DisplayStats: black: 1")) {
//         DrawTextWithCustomFont("White King is in Check", textX, (tileSize * 7) + fontSize, fontSize, RED);
//     }
//     else {
//         DrawTextWithCustomFont("White King is not in Check", textX, (tileSize * 7) + fontSize, fontSize, WHITE);
//     }
// }

void BoardStats::DisplayMoveHistory(ChessBoard chessboard) const {

    string Moves = "";
    for (int index = 0; index < chessboard.moveHistory.size(); index++) {
        Moves += chessboard.moveHistory[index];
        Moves += ", ";
        if ((index + 1) % 6 == 0) {
            Moves += "\n\n";
        }
    }
    DrawTextWithCustomFont(Moves.c_str(), 150, textY + 90, fontSize - 20, RED);
}

double BoardStats::calculateExpectedScore(double engineRating, double opponentRating) {
    return 1.0 / (1 + pow(10, (opponentRating - engineRating) / 400));
}

int BoardStats::updateEloRating(int engineRating, int opponentRating, bool engineWon) {
    double expectedScore = calculateExpectedScore(engineRating, opponentRating);
    double actualScore = engineWon ? 1.0 : 0.0;
    double kFactor = 42; // Adjust the K-factor as needed

    // Update the Elo rating based on the outcome
    return static_cast<int>(engineRating + kFactor * (actualScore - expectedScore));
}

void BoardStats::Reset() {
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
}

void BoardStats::DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) const {
    Vector2 Position = { posX, posY };
    DrawTextEx(myFont, text, Position, fontSize, 1.0, color);
}

Vector2 BoardStats::TextCenter(const char* text, float fontSize) {
    return MeasureTextEx(myFont, text, fontSize, 1.0);
}

void BoardStats::DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine) const {
    // Calculate evaluation scores for both players
    float whiteScore = Evaluate(chessboard, White, engine);
    float blackScore = Evaluate(chessboard, Black, engine);

    // Calculate proportions of white and black color for the column
    float totalScore = whiteScore + blackScore;
    float whiteProportion = (totalScore != 0) ? (whiteScore / totalScore) : 0;
    float blackProportion = (totalScore != 0) ? (blackScore / totalScore) : 0;
    //cout << whiteScore << " " << blackScore << endl;
    ostringstream White_;
    ostringstream Black_;
    White_ << fixed << setprecision(1) << whiteProportion;
    Black_ << fixed << setprecision(1) << blackProportion;
    string WhiteProp = White_.str(); // White Proportions
    string BlackProp = Black_.str(); // Black Proportions

    Rectangle columnRect = { 763, 80, 25, 640 }; // { x, y, width, height };

    // Draw the evaluation column with appropriate colors
    DrawRectangleRec(columnRect, WHITE); // Fill with white color
    Rectangle blackPart = { columnRect.x, columnRect.y, columnRect.width, columnRect.height * blackProportion };
    Rectangle whitePart = { columnRect.x, columnRect.y + blackPart.height, columnRect.width, columnRect.height * whiteProportion };
    DrawRectangleRec(whitePart, WHITE); // Fill with white color
    DrawRectangleRec(blackPart, BLACK); // Fill with black color
    DrawTextWithCustomFont(WhiteProp.c_str(), 765, 703, 15, BLACK);
    DrawTextWithCustomFont(BlackProp.c_str(), 765, 85, 15, WHITE);
}

float BoardStats::Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const {
    // Piece values
    unordered_map<int, float> pieceValues = {
        {PAWN, 1}, {KNIGHT, 3}, {BISHOP, 3}, {ROOK, 5}, {QUEEN, 9}, {KING, 10}
    };

    // Evaluate material advantage
    float selfMaterial = chessboard.calculatePlayerScore(Player) * 100,
        opponentMaterial = chessboard.calculatePlayerScore(Player == White ? Black : White) * 100;

    float materialAdvantage = selfMaterial - opponentMaterial;

    // Evaluate positional advantage using PSTs
    float positionalAdvantage = 0;
    for (int i = 0; i < Total_tiles; ++i) {

        if (chessboard.board[i].type != EMPTY) {
            int pieceValue = engine.getPSTValue(chessboard.board[i], i, Player);
            positionalAdvantage += pieceValue;
        }
    }

    // Evaluate move options and capture moves
    vector<string> moves = chessboard.GetAllPossibleMovesInChessNotation(Player);
    int moveOptions = static_cast<int>(moves.size());
    int captureMoves = 0;
    for (const auto& move : moves) {
        if (engine.IsCaptureMove(move, chessboard)) {
            captureMoves++;
        }
    }

    // Total evaluation is a combination of material, positional, move options, and capture moves
    float evaluation = abs(materialAdvantage + positionalAdvantage);
    //cout << moveOptions << " ";
    evaluation += moveOptions * 10;  // Adjust weight as needed
    evaluation += captureMoves * 20; // Adjust weight as needed
    //cout << captureMoves << endl;

    return evaluation;
}
