#include "../headers/Engine/ChessEngine.h"
#include "json/json.h" // Comment this out Before Running
//#include "jsoncpp/json/json.h"  //<----------Uncomment this before running

using namespace std;

static string SetPrecision(const float& number, const int& precision) {
    ostringstream temp;
    temp << fixed << setprecision(precision) << number;
    return temp.str();

}

//Makes a New File, or if file already exists, initialises it for for more data
static void initJson(string filename) {
    string folder = "Test_Results";
    if (!filesystem::is_directory(folder)) filesystem::create_directory(folder);
    filename = folder + "/" + filename;


    //Check if the file Exists, Create it if not, otherwise remove the last ']'
    if (!filesystem::exists(filename)) {
        cout << "Making a new .json file\n";
        ofstream NewFile(filename, ios::out | ios::app);
        NewFile.close();
    }
    else {
        ofstream jsonFile(filename, ios::ate | ios::in);
        long pos = jsonFile.tellp();
        if (pos != 0) {
            jsonFile.seekp(pos - 1);
            jsonFile << " ";
        }
        jsonFile.close();
    }
}

static void SaveToJSON(const string& fen, int depth, string timeTaken, string bestMove, string speed, int totalMoves, int numOfMovesSeen, string filename) {
    filename = "Test_Results/" + filename;

    Json::Value root;
    root["fen"] = fen;
    root["depth"] = depth;
    root["timeTaken"] = timeTaken;
    root["bestMove"] = bestMove;
    root["TotalMoves"] = totalMoves;
    root["numOfMovesSeen"] = numOfMovesSeen;
    root["Speed"] = speed;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    const string jsonString = Json::writeString(builder, root);

    ofstream outFile(filename, ios::ate | ios::in);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open " << filename << " for writing." << endl;
        return;
    }
    // Check if file is empty
    long pos = outFile.tellp();

    // If file is empty, start with '['
    if (pos != 0) {
        outFile << ", " << endl;
    }
    else {
        outFile << "[";
    }


    outFile << jsonString;
    outFile.close();
}

//Get fens from specified file
static vector<string> getFens(string filename, int numOfFENS) {
    ifstream inputFile(filename);
    vector<string> FENS;

    if (!inputFile) {
        cerr << "Error: Unable to open " << filename << endl;
        return FENS;
    }

    string line;
    int count = 0;
    while (count < numOfFENS && getline(inputFile, line)) {
        FENS.push_back(line);
        count++;
    }
    inputFile.close();

    return FENS;

}

static void endFile(string filename) {
    filename = "Test_Results/" + filename;

    ofstream outFile(filename, ios::app);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }
    outFile << "]";
    outFile.close();

}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <FEN string> <Depth>" << endl;
        return 1;
    }

    string LongestFEN = "pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp/pppppppp p pppp";
    int MaxLenght = LongestFEN.length();

    const int NumOfFENS = 100;
    const int Depth = stoi(argv[2]);
    const string filename = "analysis_results.json";
    //const string fen_file = "src/main/fens.txt";

    initJson(filename);
    string fen = argv[1];


    ChessBoard board;
    ChessEngine engine(Black, false);

    engine.state.setDepth(Depth);
    board.initializeBoardFromFEN(fen, false);

    string bestMove = engine.GenerateMove(board);
    int depth = engine.state.getDepth();
    string timeTaken = SetPrecision(Heuristics.TimeTaken, 2);
    int numOfMovesSeen = Heuristics.NumberofMovesLookedAhead;
    int totalMoves = board.GetAllPossibleMoves(Black).size();
    string speed = SetPrecision(Heuristics.Speed / 1000, 2);

    cout << left << setw(MaxLenght) << fen.substr(0, fen.length() - 1);
    cout << right << setw(5) << depth << "D   ";
    cout << timeTaken << "s   ";
    cout << bestMove << "   ";
    cout << totalMoves << "   ";
    cout << numOfMovesSeen << "   ";
    cout << speed << "kn/s" << endl;
    SaveToJSON(fen, depth, timeTaken, bestMove, speed, totalMoves, numOfMovesSeen, filename);

    endFile(filename);

    return 0;
}

//This program is for testing the engine. It should be run from the Shell(Bash) using the command below
//This program Takes every FEN string in ExtractedFENs.txt and sends it as an Argument to the Chess Engine(This Program)
//Incase you don't have ExtractedFENs.txt in the Tests folder, Run ./FenExtracter.sh
//Since every FEN is tested one by one, there are no unexpected exits and breaks. 
//Make sure you have jsoncpp and raylib installed installed
//Assuming you have jsoncpp installed in default location for Bash
//g++ -o test_engine.exe test_engine.cpp ../src/Engine/EngineState.cpp ../headers/Engine/ChessEngineStatistics.h ../src/Engine/ChessEngine.cpp ../src/Board/ChessBoard.cpp ../src/GameFlow/Flags.cpp ../src/Engine/TranspositionTables.cpp ../src/Other/GlobalVariables.cpp ../src/GameFlow/Settings.cpp ../src/Pieces/ChessPiece.cpp ../src/Pieces/Pieces.cpp ../src/Functors/ConvertNotation.cpp ../src/Exceptions/KingNotFound.cpp  -std=c++20 -ljsoncpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
