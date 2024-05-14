#include "../../headers/Statistics/BoardStats.h"

BoardStats::BoardStats() {
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
    EndMessage = "";
    whiteProportion = 0.5;
    blackProportion = 0.5;
    TimerStart = std::chrono::high_resolution_clock::now();
}

void BoardStats::DisplayEndMessage() {
    float posX = constants.textX - ((Utility::TextCenter(EndMessage.c_str(), fontSize - 10).x) / 2);
    float posY = (static_cast<float>(screenHeight) / 2) - ((Utility::TextCenter(EndMessage.c_str(), fontSize - 10).y) / 2);
    Utility::DrawTextWithCustomFont(EndMessage.c_str(), posX, posY, fontSize - 10, constants.AlertColor); // Balance Based on TextSize
}

bool BoardStats::GameIsEnded(ChessBoard& board) {
    if (!Flags::MoveIsMade())
        return (winner != NULL);
        
    int playerColor = board.isCurrentPlayerWhite() ? White : Black;
    bool isWhiteTurn = board.isCurrentPlayerWhite();

    if (board.isCheckmate()) {
        winner = isWhiteTurn ? Black : White;
        EndMessage = "Winner is ";
        EndMessage += (board.isCurrentPlayerWhite() ? constants.Black_ : constants.White_);
        Flags::GameStateChecked();
        return true;
    }

    if (board.getCheckedPlayer() == 0 && board.GetAllPossibleMoves(playerColor).empty()) {
        winner = -1; // No Winner
        EndMessage = "---Stalemate---";
        Flags::GameStateChecked();
        return true;
    }

    if (board.calculatePlayerScore(White) <= constants.MinScore && board.calculatePlayerScore(Black) <= constants.MinScore) {
        winner = -1; // No Winner
        EndMessage = "---Draw---";
        Flags::GameStateChecked();
        return true;
    }

    Flags::GameStateChecked();
    return (winner != NULL);
}


void BoardStats::DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player) {
    std::string LastMovePlayed = "";
    std::string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

    std::vector<std::string> EngineData; getData(engine, player, chessboard, EngineData);
    std::vector<std::string> moveHistory = chessboard.state.moveHistory;


    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());

    MovesAndHistory(LastMovePlayed, moveHistory);

    DisplayPlayerTitles(chessboard); //Draw Player Titles
    DrawStatistics(EngineData); //Draw engine Statistics
    DrawPlayerElos(player.ELO, engine.state.engineEloRating); // Draw Elo's
}

void BoardStats::DisplayPlayerTitles(const ChessBoard& chessboard) {
    Color CheckAlertB = chessboard.getCheckedPlayer() == Black ? constants.AlertColor : constants.messageColor;
    Color CheckAlertW = chessboard.getCheckedPlayer() == White ? constants.AlertColor : constants.messageColor;

    std::pair<float, float> w_pos = constants.get_white_pos();
    std::pair<float, float> b_pos = constants.get_black_pos();
    

    Utility::DrawTextWithCustomFont(constants.Black_, b_pos.first, b_pos.second , fontSize, CheckAlertB);
    Utility::DrawTextWithCustomFont(constants.White_, w_pos.first, w_pos.second , fontSize, CheckAlertW);
}

void BoardStats::DrawStatistics(const std::vector<std::string>& EngineData) {
    int offsetY = 70;
    const float DataFontSize = fontSize - 30;
    for (std::string message : EngineData) {
        float posX = constants.textX - (Utility::TextCenter(message.c_str(), fontSize - 30).x / 2);
        float posY = constants.textY + offsetY;
        Utility::DrawTextWithCustomFont(message.c_str(), posX, posY, DataFontSize, constants.messageColor);
        offsetY += 20;
    }
}

void BoardStats::DrawPlayerElos(int elo_W, int elo_B) {
    std::string WhiteELO = Utility::CloseInBrackets(constants.ELO_text + std::to_string(elo_W));
    std::string BLackELO = Utility::CloseInBrackets(constants.ELO_text + std::to_string(elo_B));

    //Draw ELO
    Utility::DrawTextWithCustomFont(WhiteELO.c_str(), constants.textX - (Utility::TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), static_cast<float>(screenHeight) - 146 + 30, fontSize - 30, constants.messageColor);
    Utility::DrawTextWithCustomFont(BLackELO.c_str(), constants.textX - (Utility::TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), constants.textY + 50, fontSize - 30, constants.messageColor);


}


void BoardStats::getData(const ChessEngine& engine, const User& player, const ChessBoard& chessboard, std::vector<std::string>& container){
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

    container.reserve(9);
    using namespace Utility;
    container.emplace_back("Evaluation Speed: " + SetPrecision(Speed, 1) + "kn/s");
    container.emplace_back("Saw " + AddCommas(LookAheads) + " futures in " + SetPrecision(TimeTaken, 2) + "s");
    container.emplace_back("Pruned " + AddCommas(BranchesPruned) + " Branches");
    container.emplace_back("Size: " + SetPrecision(SizeOfTable, 2) + "Mbs");
    container.emplace_back("Total Transpostions Found: " + AddCommas(FoundTranspostions));
    container.emplace_back("Depth: " + std::to_string(currentDepth));
    container.emplace_back("Evaluated " + AddCommas(evaluated) + "/" + AddCommas(moves) + " moves");
    container.emplace_back("Evaluated " + AddCommas(LookAheads) + "/" + AddCommas(totalToEvaluate) + " positions");
    container.emplace_back("Pruned " + SetPrecision(PercentagePruned, 2) + "% branches");
    

}

void BoardStats::DisplayStats(ChessBoard& chessboard) {
    //Overriden Function

    std::string LastMovePlayed = "";
    std::string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    std::vector<std::string> moveHistory = chessboard.state.moveHistory;

    if (moveHistory.size() != 0) LastMovePlayed = (moveHistory.back());
    MovesAndHistory(LastMovePlayed, moveHistory);
    DisplayPlayerTitles(chessboard);
}

void BoardStats::MovesAndHistory(std::string LastMovePlayed, const std::vector<std::string>& moveHistory) {
    float X = constants.textX - (Utility::TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2);
    float Y = static_cast<float>(screenHeight) / 2 - 42;
    Utility::DrawTextWithCustomFont(LastMovePlayed.c_str(), X, Y, fontSize - 10, constants.AlertColor);
}

void BoardStats::DisplayMoveHistory(const std::vector<std::string>& moveHistory) const {

    std::stringstream Moves;
    const size_t movesCount = moveHistory.size();
    for (size_t index = 0; index < movesCount; index++) {
        Moves << moveHistory[index];
        if ((index + 1) % 6 == 0) {
            Moves << "\n\n";
        }
        else {
            Moves << ", ";
        }
    }
    Utility::DrawTextWithCustomFont(Moves.str().c_str(), 150, constants.textY + 90, fontSize - 20, RED);
}

void BoardStats::Reset() {
    ShowMoveHistory = false;
    winner = NULL;
    SaveData = true;
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
            int pieceValue = engine.pst.getPSTValue(chessboard.board[i], i, Player);
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
    Utility::DrawTextWithCustomFont(message.c_str(), constants.textX - (Utility::TextCenter(message.c_str(), fontSize - 30).x / 2), constants.textY + 70, fontSize - 30, constants.messageColor);
}

void BoardStats::DisplayNewFENMessage(const std::string& fen) {
    std::string message = "New FEN: " + fen;
    Utility::DrawTextWithCustomFont(message.c_str(), constants.textX - (Utility::TextCenter(message.c_str(), fontSize - 30).x / 2), constants.textY + 70, fontSize - 30, constants.messageColor);
}
