#include "..\GameModes.h"

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
    float timeTaken = engine.TimeTakenForSearch;
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
