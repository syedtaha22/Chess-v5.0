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
    if (flags.MoveIsMade()) {

        cout << "Checking....." << endl;
        int playerColor = board.isCurrentPlayerWhite() ? White : Black;

        if (board.isCheckmate(board, playerColor)) {
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
        Color CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? AlertColor : messageColor;
        Color CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? AlertColor : messageColor;
    }

    if (chessboard.moveHistory.size() != 0) LastMovePlayed = (chessboard.moveHistory.back());
        
    MovesAndHistory(LastMovePlayed, chessboard);

    //Draw Player Titles
    DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
    DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertW);
    
    //Update Constantly
    int LookAheads = engine.NumberofMovesLookedAhead;
    int BranchesPruned = engine.NumberOfBranchesPruned;
    int FoundTranspostions = engine.NumberOfTranspositionsFound;
    float TimeTaken = engine.TimeTakenForSearch;
    double SizeOfTable = engine.getSizeOfTranspositionTable();
    float Speed = engine.EngineSpeed / 1000; // n/s ---> kn/s
    int currentDepth = engine.getDepth();
    
    string WhiteELO = CloseInBrackets(ELO + to_string(player.ELO));
    string BLackELO = CloseInBrackets(ELO + to_string(engine.engineEloRating));
    string SpeedMessage = "Evaluation Speed: " + SetPrecision(Speed, 1) + "kn/s";
    string BlackMessage = "Saw " + to_string(LookAheads) + " futures in " + SetPrecision(TimeTaken, 2) + "s";
    string PruningMessage = "Pruned " + to_string(BranchesPruned) + " Branches";
    string TableSizeMessage = "Size: " + SetPrecision(SizeOfTable, 2) + "Mbs";
    string TranspostionsFound = "Total Transpostions Found: " + to_string(FoundTranspostions);
    string depth = "Depth: " + to_string(currentDepth);

    DrawTextWithCustomFont(BlackMessage.c_str(), textX - (TextCenter(BlackMessage.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
    DrawTextWithCustomFont(SpeedMessage.c_str(), textX - (TextCenter(SpeedMessage.c_str(), fontSize - 30).x / 2), textY + 90, fontSize - 30, messageColor);
    DrawTextWithCustomFont(TableSizeMessage.c_str(), textX - (TextCenter(TableSizeMessage.c_str(), fontSize - 30).x / 2), textY + 110, fontSize - 30, messageColor);
    DrawTextWithCustomFont(PruningMessage.c_str(), textX - (TextCenter(PruningMessage.c_str(), fontSize - 30).x / 2), textY + 130, fontSize - 30, messageColor);
    DrawTextWithCustomFont(TranspostionsFound.c_str(), textX - (TextCenter(TranspostionsFound.c_str(), fontSize - 30).x / 2), textY + 150, fontSize - 30, messageColor);
    DrawTextWithCustomFont(depth.c_str(), textX - (TextCenter(depth.c_str(), fontSize - 30).x / 2), textY + 170, fontSize - 30, messageColor);


    //Draw ELO
    DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), screenHeight - 146 + 30, fontSize - 30, messageColor);
    DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);

}

void BoardStats::DisplayStats(ChessBoard& chessboard) {
    //Overriden Function

    string LastMovePlayed = "";
    string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

    if (flags.MoveIsMade()) {

        CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? AlertColor : messageColor;
        CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? AlertColor : messageColor;
    }

    if (chessboard.moveHistory.size() != 0) LastMovePlayed = (chessboard.moveHistory.back());
    

    MovesAndHistory(LastMovePlayed, chessboard);

    //Player Titles
    DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
    DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertB);
}

void BoardStats::MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard) {
    if (ShowMoveHistory) DisplayMoveHistory(chessboard);
    else {
        int X = textX - (TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2);
        int Y = screenHeight / 2 - 42;
        DrawTextWithCustomFont(LastMovePlayed.c_str(), X, Y, fontSize - 10, AlertColor);
    }
}

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
    if (flags.MoveIsMade()) {
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
        {PAWN, 1}, {KNIGHT, 3}, {BISHOP, 3}, {ROOK, 5}, {QUEEN, 9}, {KING, 10}
    };

    // Evaluate material advantage
    float selfMaterial = chessboard.calculatePlayerScore(Player) * 100,
      opponentMaterial = chessboard.calculatePlayerScore(Player == White ? Black : White) * 100; //100 ---> Scaling Factor

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