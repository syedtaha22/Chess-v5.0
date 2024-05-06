#include "../../headers/Other/BoardStats.h"

BoardStats::BoardStats() {
    CheckAlertB = messageColor;
    CheckAlertW = messageColor;
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
    EndMessage = "";
    whiteProportion = 0.5;
    blackProportion = 0.5;
}

void BoardStats::DisplayEndMessage() {
    float posX = textX - ((TextCenter(EndMessage.c_str(), fontSize - 10).x) / 2);
    float posY = (static_cast<float>(screenHeight) / 2) - ((TextCenter(EndMessage.c_str(), fontSize - 10).y) / 2);
    Utility::DrawTextWithCustomFont(EndMessage.c_str(), posX, posY, fontSize - 10, AlertColor); // Balance Based on TextSize
}

bool BoardStats::GameIsEnded(ChessBoard& board) {
    if (Flags::MoveIsMade()) {

        //std::cout << "Checking....." << std::endl;
        int playerColor = board.isCurrentPlayerWhite() ? White : Black;

        if (board.isCheckmate(board, playerColor)) {
            std::cout << winner << std::endl;
            EndMessage = "Winner is ";
            EndMessage += (board.isCurrentPlayerWhite() ? Black_ : White_);
            winner = board.isCurrentPlayerWhite() ? Black : White; // Set Winner

            Flags::GameStateChecked();
            //std::cout << "Check Complete" << std::endl;
            return true;
        }
        if (board.getCheckedPlayer() == 0 && board.GetAllPossibleMoves(playerColor).size() == 0) {
            EndMessage = "---Stalemate---";
            winner = -1; //No Winner

            Flags::GameStateChecked();
            //std::cout << "Check Complete" << std::endl;
            return true;
        }
        if (board.calculatePlayerScore(White) <= MinScore && board.calculatePlayerScore(Black) <= MinScore) {
            EndMessage = "---Draw---";
            winner = -1; //No Winner

            Flags::GameStateChecked();
            return true;
            //std::cout << "Check Complete" << std::endl;
        }

        //std::cout << "Check Complete" << std::endl;
        Flags::GameStateChecked();
    }



    if (winner != NULL) return true;
    else return false;

}

void BoardStats::DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player) {
    std::string LastMovePlayed = "";
    std::string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    std::vector<std::string> EngineData = getData(engine, player, chessboard);
    std::vector<std::string> moveHistory = chessboard.getMoveHistory();


    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());

    MovesAndHistory(LastMovePlayed, moveHistory);

    DisplayPlayerTitles(chessboard); //Draw Player Titles
    DrawStatistics(EngineData); //Draw engine Statistics
    DrawPlayerElos(player.ELO, engine.state.engineEloRating); // Draw Elo's
}

void BoardStats::DisplayPlayerTitles(const ChessBoard& chessboard) {
    Color CheckAlertB = chessboard.getCheckedPlayer() == Black ? AlertColor : messageColor;
    Color CheckAlertW = chessboard.getCheckedPlayer() == White ? AlertColor : messageColor;
    float b_posX = textX - (TextCenter(Black_, fontSize).x / 2);
    float b_posY = textY - (TextCenter(Black_, fontSize).y / 2) + 20;   

    float w_posX = textX - (TextCenter(White_, fontSize).x / 2);
    float w_posY = screenHeight - 146 - fontSize + 30;

    Utility::DrawTextWithCustomFont(Black_, b_posX, b_posY , fontSize, CheckAlertB);
    Utility::DrawTextWithCustomFont(White_, w_posX, w_posY , fontSize, CheckAlertW);
}

void BoardStats::DrawStatistics(const std::vector<std::string>& EngineData) {
    int offsetY = 70;
    const float DataFontSize = fontSize - 30;
    for (std::string message : EngineData) {
        float posX = textX - (TextCenter(message.c_str(), fontSize - 30).x / 2);
        float posY = textY + offsetY;
        Utility::DrawTextWithCustomFont(message.c_str(), posX, posY, DataFontSize, messageColor);
        offsetY += 20;
    }
}

void BoardStats::DrawPlayerElos(int elo_W, int elo_B) {
    std::string WhiteELO = Utility::CloseInBrackets(ELO + std::to_string(elo_W));
    std::string BLackELO = Utility::CloseInBrackets(ELO + std::to_string(elo_B));

    //Draw ELO
    Utility::DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), static_cast<float>(screenHeight) - 146 + 30, fontSize - 30, messageColor);
    Utility::DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);
}

std::vector<std::string> BoardStats::getData(const ChessEngine& engine, const User& player, const ChessBoard& chessboard){
    int LookAheads = Heuristics.NumberofMovesLookedAhead;
    int BranchesPruned = Heuristics.BranchesPruned;
    int FoundTranspostions = Heuristics.TranspositionsFound;
    float TimeTaken = static_cast<float>((Heuristics.TimeTaken));
    float SizeOfTable = static_cast<float>(engine.state.getSizeOfTranspositionTable());
    float Speed = Heuristics.Speed / 1000; // n/s ---> kn/s
    int currentDepth = Heuristics.maxDepth;
    int moves = static_cast<int>(Heuristics.totalMoves);
    int evaluated = Heuristics.movesEvaluated;
    int totalToEvaluate = static_cast<int>(Heuristics.totalMovesToEvaluate);
    float PercentagePruned = (totalToEvaluate != 0) ? (static_cast<float>(abs(LookAheads - totalToEvaluate)) / static_cast<float>(totalToEvaluate)) * 100 : 0;

    using namespace Utility;
    std::vector<std::string> DataArray = {
        "Evaluation Speed: " + SetPrecision(Speed, 1) + "kn/s",
        "Saw " + AddCommas(LookAheads) + " futures in " + SetPrecision(TimeTaken, 2) + "s",
        "Pruned " + AddCommas(BranchesPruned) + " Branches",
        "Size: " + SetPrecision(SizeOfTable, 2) + "Mbs",
        "Total Transpostions Found: " + AddCommas(FoundTranspostions),
        "Depth: " + std::to_string(currentDepth),
        "Evaluated " + AddCommas(evaluated) + "/" + AddCommas(moves) + " moves",
        "Evaluated " + AddCommas(LookAheads) + "/" + AddCommas(totalToEvaluate) + " positions",
        "Pruned " + SetPrecision(PercentagePruned, 2) + "% branches"
    };

    return DataArray;

}

void BoardStats::DisplayStats(ChessBoard& chessboard) {
    //Overriden Function

    std::string LastMovePlayed = "";
    std::string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    std::vector<std::string> moveHistory = chessboard.getMoveHistory();

    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());
    MovesAndHistory(LastMovePlayed, moveHistory);
    DisplayPlayerTitles(chessboard);
}

void BoardStats::MovesAndHistory(std::string LastMovePlayed, const std::vector<std::string>& moveHistory) {
    if (ShowMoveHistory) DisplayMoveHistory(moveHistory);
    else {
        float X = textX - (TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2);
        float Y = static_cast<float>(screenHeight) / 2 - 42;
        Utility::DrawTextWithCustomFont(LastMovePlayed.c_str(), X, Y, fontSize - 10, AlertColor);
    }
}

void BoardStats::DisplayMoveHistory(const std::vector<std::string>& moveHistory) const {

    std::string Moves = "";
    for (int index = 0; index < moveHistory.size(); index++) {
        Moves += moveHistory[index];
        Moves += ", ";
        if ((index + 1) % 6 == 0) {
            Moves += "\n\n";
        }
    }
    Utility::DrawTextWithCustomFont(Moves.c_str(), 150, textY + 90, fontSize - 20, RED);
}

double BoardStats::calculateExpectedScore(int PlayerARating, int PlayerBRating) const {

    /*      Expected Score = Qa / (Qa + Qb);
    
                Qa = 10 ^ (engineRating / c);
                Qb = 10 ^ (opponentRating / c);
                 c = Some Factor
    */

    const double c = 400.0;
    double Qa = pow(10, PlayerARating / c);
    double Qb = pow(10, PlayerBRating / c);
    double ExpectedScore = Qa / (Qa + Qb);

    return ExpectedScore;
}

int BoardStats::updateEloRating(int PlayerARating, int PlayerBRating, bool PlayerAWon) const {

    /*
            NewELO = OldELO + K * (Outcome — ExpectedOutcome),
                k = Scalling Factor
    */
    
    double expectedOutcome = calculateExpectedScore(PlayerARating, PlayerBRating);
    double actualOutcome = PlayerAWon ? 1.0 : 0.0;
    const double kFactor = 42;

    return static_cast<int>(PlayerARating + kFactor * (actualOutcome - expectedOutcome));
}

void BoardStats::Reset() {
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
}

Vector2 BoardStats::TextCenter(const char* text, float fontSize) {
    return MeasureTextEx(myFont, text, fontSize, 1.0);
}

void BoardStats::DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine) {

    //Calculate Only When a move is made
    if (Flags::MoveIsMade()) {
        float whiteScore = Evaluate(chessboard, White, engine);
        float blackScore = Evaluate(chessboard, Black, engine);

        float totalScore = whiteScore + blackScore;
        whiteProportion = (totalScore != 0) ? (whiteScore / totalScore) : 0;
        blackProportion = (totalScore != 0) ? (blackScore / totalScore) : 0;
    }
      
    std::string WhiteProp = Utility::SetPrecision(whiteProportion, 1); // White Proportions
    std::string BlackProp = Utility::SetPrecision(blackProportion, 1); // Black Proportions

    Rectangle columnRect = { 763, 80, 25, 640 }; // { x, y, width, height };

    DrawRectangleRec(columnRect, WHITE); // Fill with white color
    Rectangle blackPart = { columnRect.x, columnRect.y, columnRect.width, columnRect.height * blackProportion };
    Rectangle whitePart = { columnRect.x, columnRect.y + blackPart.height, columnRect.width, columnRect.height * whiteProportion };
    DrawRectangleRec(whitePart, WHITE); 
    DrawRectangleRec(blackPart, BLACK); 
    Utility::DrawTextWithCustomFont(WhiteProp.c_str(), 765, 703, 15, BLACK);
    Utility::DrawTextWithCustomFont(BlackProp.c_str(), 765, 85, 15, WHITE);
}

float BoardStats::Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const {
    std::unordered_map<int, float> pieceValues = {
        {PAWN, 1.0f}, {KNIGHT, 3.0f}, {BISHOP, 3.0f}, {ROOK, 5.0f}, {QUEEN, 9.0f}, {KING, 10.0f}
    };

    // Evaluate material advantage
    float selfMaterial = static_cast<float>(chessboard.calculatePlayerScore(Player)) * 100,
      opponentMaterial = static_cast<float>(chessboard.calculatePlayerScore(Player == White ? Black : White))* 100; //100 ---> Scaling Factor

    float materialAdvantage = selfMaterial - opponentMaterial;

    // Evaluate positional advantage using PSTs
    float positionalAdvantage = 0;
    for (int i = 0; i < Total_tiles; ++i) {

        if (chessboard.board[i]->type != EMPTY) {
            int pieceValue = engine.getPSTValue(chessboard.board[i], i, Player);
            positionalAdvantage += pieceValue;
        }
    }

    // Evaluate move options and capture moves
    std::vector<std::string> moves = chessboard.GetAllPossibleMovesInChessNotation(Player);
    int moveOptions = static_cast<int>(moves.size());
    int captureMoves = 0;
    for (const auto& move : moves) {
        if (IsCaptureMove()(move, chessboard)) {
            captureMoves++;
        }
    }

    
    float evaluation = abs(materialAdvantage + positionalAdvantage);
    
    evaluation += moveOptions * 10;  // Adjust weight as needed
    evaluation += captureMoves * 20; // Adjust weight as needed

    return evaluation;
}

void BoardStats::DisplayNewDepthMessage(const int& newdepth) {
    std::string message = "New Depth: " + std::to_string(newdepth);
    Utility::DrawTextWithCustomFont(message.c_str(), textX - (TextCenter(message.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
}

void BoardStats::DisplayNewFENMessage(const std::string& fen) {
    std::string message = "New FEN: " + fen;
    Utility::DrawTextWithCustomFont(message.c_str(), textX - (TextCenter(message.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
}
