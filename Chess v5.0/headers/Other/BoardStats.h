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

    float whiteProportion;
    float blackProportion;

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
    void DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player);
    void DisplayStats(ChessBoard& chessboard);
    void MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard);
    void Reset();
    void DisplayMoveHistory(ChessBoard chessboard) const;
    void DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine);
    void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) const;

    string CloseInBrackets(string stringClose);
    string SetPrecision(const float& number, const int& precision) const;

    bool GameIsEnded(ChessBoard& board);

    int updateEloRating(int PlayerARating, int PlayerBRating, bool engineWon) const;
    float Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const;
    double calculateExpectedScore(int PlayerARating, int PlayerBRating) const;

    Vector2 TextCenter(const char* text, float fontSize);

    void DisplayNewDepthMessage(const int& newdepth);
    void DisplayNewFENMessage(const string& fen);
};



#endif // !BOARDSTATS_H


