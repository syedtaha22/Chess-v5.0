#ifndef BOARDSTATS_H
#define BOARDSTATS_H

#include "User.h"

class BoardStats {
private:

    //Set a const Text Size for display
    const float TextSize = 50;

    //Set a Reference point based on the Dimensions of The information Box on GUI
    const float textX = (InfoBoxWidth / 2) + InfoBoxX;
    const float textY = 100;

    //Calulate the Height and Width of Board
    const int BoardDimensions = tileSize * boardSize;

    //Colors
    const Color messageColor = WHITE;
    const Color AlertColor = RED;
    Color CheckAlertB;
    Color CheckAlertW;

    //Evaluation Column
    float whiteProportion;
    float blackProportion;

    //Constant Messages, so that we don't have to right them each time
    const char* BlackInCheck = "Black's King is in Check";
    const char* WhiteInCheck = "White's King is in Check";
    const char* ELO = "ELO: ";
    const char* Black_ = "Black";
    const char* White_ = "White";

    std::chrono::high_resolution_clock::time_point TimerStart = high_resolution_clock::now();

    const int MinScore = 10; // Kings Score

public:


    //Wether to Show Move History or Not
    bool ShowMoveHistory;
    bool SaveData;

    //Decide Winner
    int winner;
    //Message Regarding Who won
    string EndMessage;

    BoardStats();

    void DisplayEndMessage();

    //Display Game Statistics in Single Player Mode
    void DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player);

    //Display Game Statistice in Multiplayer Mode
    void DisplayStats(ChessBoard& chessboard);

    //Display Last Move or Move History based on flag
    void MovesAndHistory(string LastMovePlayed, const std::vector<std::string>& moveHistory);

    std::vector<std::string> getData(const ChessEngine& engine, const User& player, const ChessBoard& chessboard);
    void DrawPlayerElos(int elo_W, int elo_B);

    void DrawStatistics(const std::vector<std::string>& EngineData);
    void DisplayPlayerTitles(const ChessBoard& chessboard);

    //Reset Stats
    void Reset();

    //Displays the Move History
    void DisplayMoveHistory(const std::vector<std::string>& moveHistory) const;

    void DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine);
    float Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const;

    //Draw Text with Roboto font 
    void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) const;


    //Close a String Between Brackets
    string CloseInBrackets(string stringClose);

    string SetPrecision(const float& number, const int& precision) const;

    //Check if Gamr ended or not
    bool GameIsEnded(ChessBoard& board);


    int updateEloRating(int PlayerARating, int PlayerBRating, bool engineWon) const;
    double calculateExpectedScore(int PlayerARating, int PlayerBRating) const;

    //Calculate and Return the center of a text string
    Vector2 TextCenter(const char* text, float fontSize);

    //Functions related to FENs
    void DisplayNewDepthMessage(const int& newdepth);
    void DisplayNewFENMessage(const string& fen);

    // format(xxxxxxxxx) --> xxx,xxx,xxx
    string format(int number);

    
};



#endif // !BOARDSTATS_H


