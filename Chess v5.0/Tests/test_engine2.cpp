#include "../headers/Engine/ChessEngine.h"


string SetPrecision(const float& number, const int& precision) {
    ostringstream temp;
    temp << fixed << setprecision(precision) << number;
    return temp.str();

}

int main(int argc, char* argv[]) {
   
    string LongestFEN = "pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp p pppp";
    int MaxLenght = LongestFEN.length();


    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <FEN string>" << endl;
        return 1;
    }

    // Extract the FEN string from command-line arguments
    string fen = argv[1];

    // Initialize ChessBoard and ChessEngine
    ChessBoard board;
    ChessEngine engine(Black);
    board.initializeBoardFromFEN(fen);

    // Generate the best move
    string bestMove = engine.GenerateMove(board);

    // Get other information from the ChessEngine
    int depth = engine.getDepth();
    string timeTaken = SetPrecision(engine.TimeTakenForSearch, 2);
    int numOfMovesSeen = engine.NumberofMovesLookedAhead;
    int TotalMoves = board.GetAllPossibleMoves(Black).size();

    // Display the information on the terminal
    cout << left << setw(MaxLenght) << fen << right << setw(5) << depth << "D   " << timeTaken << "s   " << bestMove << "   " << TotalMoves << "   " << numOfMovesSeen << endl;

    // Save the information to a file
    ofstream outFile("analysis_results.txt", ios::app);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return 1;
    }

    outFile << left << setw(MaxLenght) << fen << right << setw(5) << depth << "D   " << timeTaken << "s   " << bestMove << "   " << TotalMoves << "   " << numOfMovesSeen << endl;
    outFile.close();

    return 0;
}


//g++ -o test_engine.exe test_engine2.cpp ../src/Engine/ChessEngine.cpp ../src/Board/ChessBoard.cpp ../src/Engine/TranspositionTables.cpp ../src/Other/GlobalVariables.cpp ../src/Pieces/ChessPiece.cpp -std=c++20
