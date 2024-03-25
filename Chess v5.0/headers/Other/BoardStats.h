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

    std::string format(int number) {
        std::string str = std::to_string(number);
        int len = static_cast<int>(str.length());
        if (len <= 3) // No need for formatting if the number is less than 1000
            return str;

        int commas = (len - 1) / 3; // Calculate the number of commas to be inserted
        std::string formatted;
        formatted.reserve(len + commas);

        int i = 0;
        int j = len % 3 == 0 ? 3 : len % 3; // Adjust for the first segment
        formatted.append(str, 0, j);

        for (; i + j < len; i += j, j = 3) {
            formatted.push_back(',');
            formatted.append(str, i + j, 3);
        }

        return formatted;
    }
};



#endif // !BOARDSTATS_H


