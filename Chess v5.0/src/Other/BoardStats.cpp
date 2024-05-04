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
    DrawTextWithCustomFont(EndMessage.c_str(), posX, posY, fontSize - 10, AlertColor); // Balance Based on TextSize
}

bool BoardStats::GameIsEnded(ChessBoard& board) {
    if (Flags::MoveIsMade()) {

        cout << "Checking....." << endl;
        int playerColor = board.isCurrentPlayerWhite() ? White : Black;

        if (board.isCheckmate(board, playerColor)) {
            cout << winner << endl;
            EndMessage = "Winner is ";
            EndMessage += (board.isCurrentPlayerWhite() ? Black_ : White_);
            winner = board.isCurrentPlayerWhite() ? Black : White; // Set Winner

            Flags::GameStateChecked();
            cout << "Check Complete" << endl;
            return true;
        }
        if (board.getCheckedPlayer() == 0 && board.GetAllPossibleMoves(playerColor).size() == 0) {
            EndMessage = "---Stalemate---";
            winner = -1; //No Winner

            Flags::GameStateChecked();
            cout << "Check Complete" << endl;
            return true;
        }
        if (board.calculatePlayerScore(White) <= board.getPieceScore(KING) && board.calculatePlayerScore(Black) <= board.getPieceScore(KING)) {
            EndMessage = "---Draw---";
            winner = -1; //No Winner

            Flags::GameStateChecked();
            return true;
            cout << "Check Complete" << endl;
        }

        cout << "Check Complete" << endl;
        Flags::GameStateChecked();
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
    std::vector<std::string> EngineData = getData(engine, player, chessboard);
    std::vector<std::string> moveHistory = chessboard.getMoveHistory();

    if (chessboard.moveHistory.size() != 0) LastMovePlayed = (chessboard.moveHistory.back());
        
    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());

    MovesAndHistory(LastMovePlayed, moveHistory);

    DisplayPlayerTitles(chessboard); //Draw Player Titles
    DrawStatistics(EngineData); //Draw engine Statistics
    DrawPlayerElos(player.ELO, engine.engineEloRating); // Draw Elo's
}

void BoardStats::DisplayPlayerTitles(const ChessBoard& chessboard) {
    Color CheckAlertB = chessboard.getCheckedPlayer() == Black ? AlertColor : messageColor;
    Color CheckAlertW = chessboard.getCheckedPlayer() == White ? AlertColor : messageColor;
    float b_posX = textX - (TextCenter(Black_, fontSize).x / 2);
    float b_posY = textY - (TextCenter(Black_, fontSize).y / 2) + 20;   

    float w_posX = textX - (TextCenter(White_, fontSize).x / 2);
    float w_posY = screenHeight - 146 - fontSize + 30;

    DrawTextWithCustomFont(Black_, b_posX, b_posY , fontSize, CheckAlertB);
    DrawTextWithCustomFont(White_, w_posX, w_posY , fontSize, CheckAlertW);
}

void BoardStats::DrawStatistics(const std::vector<std::string>& EngineData) {
    int offsetY = 70;
    const float DataFontSize = fontSize - 30;
    for (std::string message : EngineData) {
        float posX = textX - (TextCenter(message.c_str(), fontSize - 30).x / 2);
        float posY = textY + offsetY;
        DrawTextWithCustomFont(message.c_str(), posX, posY, DataFontSize, messageColor);
        offsetY += 20;
    }
}

void BoardStats::DrawPlayerElos(int elo_W, int elo_B) {
    std::string WhiteELO = CloseInBrackets(ELO + to_string(elo_W));
    std::string BLackELO = CloseInBrackets(ELO + to_string(elo_B));

    //Draw ELO
    DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), static_cast<float>(screenHeight) - 146 + 30, fontSize - 30, messageColor);
    DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);
}

std::vector<std::string> BoardStats::getData(const ChessEngine& engine, const User& player, const ChessBoard& chessboard){
    int LookAheads = Heuristics.NumberofMovesLookedAhead;
    int BranchesPruned = Heuristics.BranchesPruned;
    int FoundTranspostions = Heuristics.TranspositionsFound;
    float TimeTaken = static_cast<float>((Heuristics.TimeTaken));
    float SizeOfTable = static_cast<float>(engine.getSizeOfTranspositionTable());
    float Speed = Heuristics.Speed / 1000; // n/s ---> kn/s
    int currentDepth = Heuristics.currentDepth;
    int moves = static_cast<int>(Heuristics.totalMoves);
    int evaluated = Heuristics.movesEvaluated;
    int totalToEvaluate = static_cast<int>(Heuristics.totalMovesToEvaluate);
    float PercentagePruned = (totalToEvaluate != 0) ? (static_cast<float>(abs(LookAheads - totalToEvaluate)) / static_cast<float>(totalToEvaluate)) * 100 : 0;


    std::vector<std::string> DataArray = {
        "Evaluation Speed: " + SetPrecision(Speed, 1) + "kn/s",
        "Saw " + format(LookAheads) + " futures in " + SetPrecision(TimeTaken, 2) + "s",
        "Pruned " + format(BranchesPruned) + " Branches",
        "Size: " + SetPrecision(SizeOfTable, 2) + "Mbs",
        "Total Transpostions Found: " + format(FoundTranspostions),
        "Depth: " + to_string(currentDepth),
        "Evaluated " + format(evaluated) + "/" + format(moves) + " moves",
        "Evaluated " + format(LookAheads) + "/" + format(totalToEvaluate) + " positions",
        "Pruned " + SetPrecision(PercentagePruned, 2) + "% branches"
    };

    return DataArray;

}

void BoardStats::DisplayStats(ChessBoard& chessboard) {
    //Overriden Function

    string LastMovePlayed = "";
    string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    std::vector<std::string> moveHistory = chessboard.getMoveHistory();

    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());
    MovesAndHistory(LastMovePlayed, moveHistory);
    DisplayPlayerTitles(chessboard);
}

void BoardStats::MovesAndHistory(string LastMovePlayed, const std::vector<std::string>& moveHistory) {
    if (ShowMoveHistory) DisplayMoveHistory(moveHistory);
    else {
        float X = textX - (TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2);
        float Y = static_cast<float>(screenHeight) / 2 - 42;
        DrawTextWithCustomFont(LastMovePlayed.c_str(), X, Y, fontSize - 10, AlertColor);
    }
}

void BoardStats::DisplayMoveHistory(const std::vector<std::string>& moveHistory) const {

    string Moves = "";
    for (int index = 0; index < moveHistory.size(); index++) {
        Moves += moveHistory[index];
        Moves += ", ";
        if ((index + 1) % 6 == 0) {
            Moves += "\n\n";
        }
    }
    DrawTextWithCustomFont(Moves.c_str(), 150, textY + 90, fontSize - 20, RED);
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

void BoardStats::DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) const {
    Vector2 Position = { posX, posY };
    DrawTextEx(myFont, text, Position, fontSize, 1.0, color);
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
      
    string WhiteProp = SetPrecision(whiteProportion, 1); // White Proportions
    string BlackProp = SetPrecision(blackProportion, 1); // Black Proportions

    Rectangle columnRect = { 763, 80, 25, 640 }; // { x, y, width, height };

    DrawRectangleRec(columnRect, WHITE); // Fill with white color
    Rectangle blackPart = { columnRect.x, columnRect.y, columnRect.width, columnRect.height * blackProportion };
    Rectangle whitePart = { columnRect.x, columnRect.y + blackPart.height, columnRect.width, columnRect.height * whiteProportion };
    DrawRectangleRec(whitePart, WHITE); 
    DrawRectangleRec(blackPart, BLACK); 
    DrawTextWithCustomFont(WhiteProp.c_str(), 765, 703, 15, BLACK);
    DrawTextWithCustomFont(BlackProp.c_str(), 765, 85, 15, WHITE);
}

float BoardStats::Evaluate(const ChessBoard& chessboard, int Player, ChessEngine& engine) const {
    unordered_map<int, float> pieceValues = {
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
    vector<string> moves = chessboard.GetAllPossibleMovesInChessNotation(Player);
    int moveOptions = static_cast<int>(moves.size());
    int captureMoves = 0;
    for (const auto& move : moves) {
        if (engine.IsCaptureMove(move, chessboard)) {
            captureMoves++;
        }
    }

    
    float evaluation = abs(materialAdvantage + positionalAdvantage);
    
    evaluation += moveOptions * 10;  // Adjust weight as needed
    evaluation += captureMoves * 20; // Adjust weight as needed

    return evaluation;
}

string BoardStats::SetPrecision(const float& number, const int& precision) const {
    ostringstream temp;
    temp << fixed << setprecision(precision) << number;
    return temp.str();
   
}

void BoardStats::DisplayNewDepthMessage(const int& newdepth) {
    string message = "New Depth: " + to_string(newdepth);
    DrawTextWithCustomFont(message.c_str(), textX - (TextCenter(message.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
}

void BoardStats::DisplayNewFENMessage(const string& fen) {
    string message = "New Depth: " + fen;
    DrawTextWithCustomFont(message.c_str(), textX - (TextCenter(message.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
}
std::string BoardStats::format(int number) {
    std::string str = std::to_string(number);
    size_t len = str.length();
    if (len <= 3) // No need for formatting if the number is less than 1000
        return str;

    size_t commas = (len - 1) / 3; // Calculate the number of commas to be inserted
    std::string formatted;
    formatted.reserve(len + commas);

    size_t i = 0;
    size_t j = len % 3 == 0 ? 3 : len % 3; // Adjust for the first segment

    formatted.append(str, 0, j);

    for (; i + j < len; i += j, j = 3) {
        formatted.push_back(',');
        formatted.append(str, i + j, 3);
    }

    return formatted;
}