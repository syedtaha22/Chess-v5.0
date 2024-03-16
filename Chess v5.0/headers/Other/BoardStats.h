#ifndef BOARDSTATS_H
#define BOARDSTATS_H

#include "User.h"

class BoardStats {
private:
    const float TextSize = 50;

    const float textX = (InfoBoxWidth / 2) + InfoBoxX;
    const float textY = 100;
    const int BoardDimensions = tileSize * boardSize;
    const Color messageColor = WHITE;
    const Color AlertColor = RED;

    Color CheckAlertB;
    Color CheckAlertW;

    const char* BlackInCheck = "Black's King is in Check";
    const char* WhiteInCheck = "White's King is in Check";
    const char* ELO = "ELO: ";
    const char* Black_ = "Black";
    const char* White_ = "White";
    Flags flags;


public:
    bool ShowMoveHistory;
    bool SaveData;
    int winner;
    string EndMessage;

    BoardStats();

    void DisplayEndMessage();

    bool GameIsEnded(ChessBoard& board);

    string CloseInBrackets(string stringClose);

    void DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player);

    void DisplayStats(ChessBoard& chessboard);

    void MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard);

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

    void DisplayMoveHistory(ChessBoard chessboard) const;

    double calculateExpectedScore(double engineRating, double opponentRating);

    int updateEloRating(int engineRating, int opponentRating, bool engineWon);

    void Reset();

    void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) const;

    Vector2 TextCenter(const char* text, float fontSize);

    void DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine) const;

    float Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const;

    string SetPrecision(const float& number, const int& precision) const;
};



#endif // !BOARDSTATS_H


