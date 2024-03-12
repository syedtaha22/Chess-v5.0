#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <limits>
//#include<atomic>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

using namespace std;
using namespace chrono;

Sound ChessPiecePlaced;
Sound ChessPieceCaptured;
Sound KingChecked;
Sound GameStarts;
Sound GameEnds;

string ChessPiecePlacedFile = "Resources\\Audio\\ChessPiecePlaced.wav";
string ChessPieceCapturedFile = "Resources\\Audio\\ChessPieceCaptured.wav";
string KingCheckedFile = "Resources\\Audio\\KingChecked.wav";
string GameStartsFile = "Resources\\Audio\\GameStart.wav";
string GameEndsFile = "Resources\\Audio\\GameEnd.wav";
string fontFile = "Resources\\Fonts\\Roboto\\Roboto-Regular.ttf";
string programIcon = "Resources\\BlackPieces\\bn.png";

Font myFont; 

const int screenWidth = 1500;
const int screenHeight = 800;
const int boardSize = 8;
const int tileSize = 80;
const int MaxHistorySize = 100;
const int ReverseOffset = 7;

const float fontSize = 50;

int BoardOffsetX = 800;
int BoardOffsetY = 80;

int Segments = 20;

float Roundedness = 0.02f;
float buttonWidth = 200;
float buttonHeight = 70;
float buttonCenterX = buttonWidth / 2;
float buttonCenterY = buttonHeight / 2;
float InfoBoxX = 60;
float InfoBoxY = 80;
float InfoBoxWidth = 610;
float InfoBoxHeight = 640;

bool DoneOnceperMove = false;

Rectangle PawnPromotionDialogue;

Color lightSquare = { 238, 238, 210, 255 };
Color darkSquare = { 118, 150, 86, 255 };
Color Transparent = { 0, 0, 0, 0 };
Color Background = { 49, 46, 43, 255 };
Color Translucent = { 0, 0, 0, 64 };
// Color Background = { 0,0,0,0 };

// Color MoveHighlight = { 187, 204, 68, 255};
Color MoveHighlight = Fade({ 255, 255, 0, 200 }, 0.5);
Color MovesForPieceHighLight = Fade({ 255, 0, 0, 200 }, 0.5);
Color PreMoveHighlight = Fade({ 0, 0, 255, 200 }, 0.5);
Color NextMoveHighlight = Fade({ 0, 255, 0, 200 }, 0.5);

//bool gameStarted = false;
//bool isMultiplayerGame = false; // Multiplayer
//bool isSinglePlayer = false;        // AI game mode

int offsets;

const int EMPTY = 0;
const int KING = 1;
const int PAWN = 2;
const int KNIGHT = 3;
const int BISHOP = 4;
const int ROOK = 5;
const int QUEEN = 6;

const int White = 8;
const int Black = 16;

const int Total_tiles = 64;
enum class PromotionPiece
{
    QUEEN,
    ROOK,
    KNIGHT,
    BISHOP
};

class ReadWrite
{

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};


class Flags {

    static bool isMoveMade;
    static bool SinglePlayer;
    static bool MultiplayerGame;
    static bool gameStarted;
public:

    Flags() {}

    static void CheckGameState() {
        isMoveMade = true;
    }

    static void GameStateChecked() {
        isMoveMade = false;
    }

    static bool MoveIsMade() {
        return isMoveMade;
    }

    static void StartGame() {
        gameStarted = true;
    }

    static bool isMultiplayerGame() {
        return MultiplayerGame;
    }
    
    static bool isSinglePlayer() {
        return SinglePlayer;
    }

    static bool isGameStarted() {
        return gameStarted;
    }

    static void EndGame() {
        gameStarted = false;
        SinglePlayer = false;
        MultiplayerGame = false;
    }

    static void SinglePlayerMode() {
        SinglePlayer = true;
    }

    static void MultiplayerMode() {
        MultiplayerGame = true;
    }

    static void DisableSinglePlayer() {
        SinglePlayer = false;
    }

    

};

bool Flags::isMoveMade = false;
bool Flags::SinglePlayer = false;
bool Flags::MultiplayerGame = false;
bool Flags::gameStarted = false;



class ChessPiece{
public:
    int type;
    int color;
    bool firstMove;
    bool isSlidingPiece;
    Texture2D texture;
    Rectangle rectangle;
    bool isDragged;
    bool canCastle;
    bool canCastleQueenSide;
    bool canCastleKingSide;
    int PieceCode;

    //Piece Type Codes:
    //White King   = 9
    //White Pawn   = 10
    //White Knigh  = 11
    //White Bishop = 12
    //White Rook   = 13
    //White Queen  = 14   
     
    //Black King   = 17
    //Black Pawn   = 18
    //Black Knigh  = 19
    //Black Bishop = 20
    //Black Rook   = 21
    //Black Queen  = 22


    ChessPiece(int pieceType = EMPTY, int pieceColor = EMPTY, bool isFirstMove = true) : type(pieceType), color(pieceColor), firstMove(true){

            PieceCode = pieceColor | pieceType;
            isSlidingPiece = (type == ROOK) || (type == BISHOP) || (type == QUEEN);
            AssignTextures();
            rectangle.width = tileSize;
            rectangle.height = tileSize;
            isDragged = false;
            canCastleQueenSide = false;
            canCastleKingSide = false;

        
    }

    void AssignTextures(){
  
        if (type != EMPTY){
            string fileName = "Resources\\";
            fileName += (color == White) ? "WhitePieces\\" : "BlackPieces\\";
            int temp;
            temp = color | type;
            fileName += to_string(PieceCode);
            fileName += ".png";
            texture = LoadTexture(fileName.c_str());
        }
    }

    void DestroyTextures(){
        if (type != EMPTY){
            cout << "Unloaded Texture for: " << color << " " << type << endl;
            UnloadTexture(texture);
        }
    }
};

class ChessBoard{

private:
    vector<int> SlidingPieceMoveData;
    vector<int> PawnMoveData;
    vector<int> KnightMoveData;
    vector<int> KingMoveData;

    vector<int> MovesForSelectedPiece;
    vector<int> OpponentMoves;

    bool currentPlayerIsWhite;
    bool DoCastle;
    bool PieceIsCaptured;
    pair<int, int> MoveIndices;
    // Scores for each piece type
    const int pawnScore = 1;
    const int rookScore = 5;
    const int knightScore = 3;
    const int bishopScore = 3;
    const int queenScore = 9;
    const int kingScore = 100;
    Flags flags;

public:
    ChessPiece board[Total_tiles];
    vector<string> moveHistory;
    bool isBoardReversed;

    ChessBoard(){
        currentPlayerIsWhite = true;
        for (auto chesspiece : board){
            chesspiece = ChessPiece(EMPTY, EMPTY);
        }
        DoCastle = false;
        MoveIndices = { -1, -1 };
        isBoardReversed = false;
        PieceIsCaptured = false;
    }

    int getPieceScore(const ChessPiece& piece) const{
        switch (piece.type) {
            case PAWN: return pawnScore;
            case ROOK: return rookScore;
            case KNIGHT: return knightScore;
            case BISHOP: return bishopScore;
            case QUEEN: return queenScore;
            case KING: return kingScore;
            default: return 0; // Unknown piece type
        }
    }

    void AddMoveToHistory(string move){
        moveHistory.push_back(move);
    }

    void promotePawn(int toTile){
        bool PawnPromoted = false;
        // cout << "\nPawn promotion! Choose a piece (Q, R, N, B): ";

        //ChessPiece promotedPiece;
        //
        // if (IsKeyPressed(KEY_Q)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    PawnPromoted = false;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_R)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? ROOK) : tolower(ROOK), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Rook" << endl;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_N)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? KNIGHT) : tolower(KNIGHT), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Knight" << endl;
        //    //break;
        //}
        // else if (IsKeyPressed(KEY_B)) {
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? BISHOP) : tolower(BISHOP), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Bishop" << endl;
        //    //break;
        //}
        // else {
        //    cout << "Invalid choice. Defaulting to Queen promotion." << endl;
        //    promotedPiece = ChessPiece(currentPlayerIsWhite ? QUEEN) : tolower(QUEEN), currentPlayerIsWhite ? 'W' : 'B');
        //    cout << "Piece Promoted to Queen" << endl;
        //    //break;
        //}

        ChessPiece promotedPiece = ChessPiece(QUEEN, currentPlayerIsWhite ? White : Black);
        // cout << "Piece Promoted to Queen" << endl;
        // board[toTile].DestroyTextures();
        //  Update the board with the promoted piece
        board[toTile] = promotedPiece;
        SetPiecePositions();
    }

    vector<int> FilterValidMoves(int fromIndex, vector<int> possibleMoves) const{
        ChessPiece piece = board[fromIndex];

        // Filter out moves that leave the king in check
        vector<int> filteredMoves;
        for (int toTile : possibleMoves){
            // Simulate the move on a temporary board
            ChessBoard tempBoard = *this;
            tempBoard.MakeMove(fromIndex, toTile);

            // Check if the king is in check after the move
            if (!tempBoard.isCheck(tempBoard, piece.color, "Board: Filter Valid Moves")){
                filteredMoves.push_back(toTile);
            }
        }
        return filteredMoves;
    }

    int calculatePlayerScore(char playerColor) const{
        int totalScore = 0;
        for (int i = 0; i < Total_tiles; i++){
            if (board[i].color == playerColor){
                totalScore += getPieceScore(board[i]);
            }
        }
        return totalScore;
    }

    void initializeBoardFromFEN(const string& fen) {
        moveHistory.clear();
        MoveIndices = { -1,-1 };
        int row = 0, col = 0;
        // Parse the FEN string
        for (char symbol : fen) {
            if (symbol == ' ') {
                break;  // Stop parsing after the board section
            }
            if (symbol == '/') {
                row++;
                col = 0;
            }
            else if (isdigit(symbol)) {
                // Fill empty squares
                int numEmptySquares = symbol - '0';
                for (int i = 0; i < numEmptySquares; i++) {
                    board[row * 8 + col] = ChessPiece(EMPTY, EMPTY);
                    col++;
                }
            }
            else {
                // Set piece based on FEN character
                int pieceType = EMPTY;
                int pieceColor = (isupper(symbol) ? White : Black);
                switch (toupper(symbol)) {
                    case 'K': pieceType = KING; break;
                    case 'P': pieceType = PAWN; break;
                    case 'N': pieceType = KNIGHT; break;
                    case 'B': pieceType = BISHOP; break;
                    case 'R': pieceType = ROOK; break;
                    case 'Q': pieceType = QUEEN; break;
                    default: break;
                }
                board[row * 8 + col] = ChessPiece(pieceType, pieceColor);
                col++;
            }
        }
        // Set the current player based on FEN
        currentPlayerIsWhite = (fen.find("w") != string::npos);
        int WhiteKingIndex = GetKingIndex(White);
        int BlackKingIndex = GetKingIndex(Black);
        size_t castlingRightsPos = fen.find_last_of(" ");
        if (castlingRightsPos != string::npos && castlingRightsPos + 1 < fen.length()) {
            string castlingRights = fen.substr(castlingRightsPos + 1);
            for (char c : castlingRights) {
                if (c == 'K') {
                    board[WhiteKingIndex].canCastleKingSide = true;
                }
                else if (c == 'Q') {
                    board[WhiteKingIndex].canCastleQueenSide = true;
                }
                else if (c == 'k') {
                    board[BlackKingIndex].canCastleKingSide = true;
                }
                else if (c == 'q') {
                    board[BlackKingIndex].canCastleQueenSide = true;
                }
            }
        }

        // Set piece positions and additional FEN features if needed
        SetPiecePositions();
        // ...code for additional FEN features
    }

    int GetKingIndex(const int& playercolor) const {
        for (int index = 0; index < Total_tiles; index++) {
            if (board[index].type == KING && board[index].color == playercolor) {
                return index;
            }
        }

        cout << "\n\nGetKingIndex: KING not Found\n\n";
        exit(404);

    }

    // Function to generate and save FEN string to file

    void saveMatchHistoryToFile() const {
        // Open file for writing (in append mode)
        ofstream file("MatchHistory.txt", ios::app);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        // Generate FEN string from current board state
        string fen;
        for (int row = 0; row < 8; row++) {
            int emptyCount = 0;
            for (int col = 0; col < 8; col++) {
                int index = row * 8 + col;
                if (board[index].type == EMPTY) {
                    emptyCount++;
                }
                else {
                    if (emptyCount > 0) {
                        fen += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    switch (board[index].type) {
                    case KING:
                        fen += (board[index].color == White) ? 'K' : 'k';
                        break;
                    case PAWN:
                        fen += (board[index].color == White) ? 'P' : 'p';
                        break;
                    case KNIGHT:
                        fen += (board[index].color == White) ? 'N' : 'n';
                        break;
                    case BISHOP:
                        fen += (board[index].color == White) ? 'B' : 'b';
                        break;
                    case ROOK:
                        fen += (board[index].color == White) ? 'R' : 'r';
                        break;
                    case QUEEN:
                        fen += (board[index].color == White) ? 'Q' : 'q';
                        break;
                    default:
                        // Handle unexpected piece type
                        cerr << "Error: Unexpected piece type encountered." << endl;
                        break;
                    }
                }
            }
            if (emptyCount > 0) {
                fen += to_string(emptyCount);
            }
            if (row < 7) {
                fen += '/';
            }
        }
        fen += (currentPlayerIsWhite ? " w " : " b ");
        // Add additional FEN features if needed

        // Write FEN string to file
        file << fen << endl; // Append newline after each FEN string
        // Close file
        file.close();

        cout << "Saved FEN string" << endl;
    }

    void InitializeDefaultBoard(){

        currentPlayerIsWhite = true;
        // Set up the board with pieces in their initial positions
        string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        initializeBoardFromFEN(DefaultFEN);
    }

    void initializeBoard() {
        // Set up the board with pieces in their initial positions

        // Black pieces
        board[0] = ChessPiece(ROOK, Black);
        board[1] = ChessPiece(KNIGHT, Black);
        board[2] = ChessPiece(BISHOP, Black);
        board[3] = ChessPiece(QUEEN, Black);
        board[4] = ChessPiece(KING, Black);
        board[5] = ChessPiece(BISHOP, Black);
        board[6] = ChessPiece(KNIGHT, Black);
        board[7] = ChessPiece(ROOK, Black);
        for (int i = 8; i <= 15; i++) {
            board[i] = ChessPiece(PAWN, Black);
        }

        // Empty spaces in the middle
        for (int i = 16; i <= 47; i++) {
            board[i] = ChessPiece(EMPTY, EMPTY);

        }

        // White pieces
        for (int i = 48; i <= 55; i++) {
            board[i] = ChessPiece(PAWN, White);
        }
        board[56] = ChessPiece(ROOK, White);
        board[57] = ChessPiece(KNIGHT, White);
        board[58] = ChessPiece(BISHOP, White);
        board[59] = ChessPiece(QUEEN, White);
        board[60] = ChessPiece(KING, White);
        board[61] = ChessPiece(BISHOP, White);
        board[62] = ChessPiece(KNIGHT, White);
        board[63] = ChessPiece(ROOK, White);
        SetPiecePositions();
    }

    void DisplayBoard() const{
        cout << "8 ";
        for (int i = 0; i < Total_tiles; i++)
        {
            // cout << (i/8) + 1  << " ";
            cout << "[" << board[i].type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles)
            {
                cout << endl;
                cout << 8 - (i + 1) / 8 << " ";
            }

            // cout << endl;
        }
        cout << endl;
        cout << "   a  b  c  d  e  f  g  h" << endl;
    }

    pair<int, int> PieceCoordinates(int pieceIndex) const{
        if (pieceIndex < 0 || pieceIndex >= Total_tiles){
            return make_pair(-1, -1);
        }
        int rank = pieceIndex / boardSize;
        int file = pieceIndex % boardSize;
        return make_pair(rank, file);
    }

    void SetPiecePositions(){
        // Set initial positions of the pieces
        for (int index = 0; index < Total_tiles; index++){
            pair<int, int> PieceCoords = PieceCoordinates(index);
            board[index].rectangle.x = BoardOffsetX + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize);
            board[index].rectangle.y = BoardOffsetY + static_cast<float>((abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize);
        }
    }

    void DrawBoard() const{
        for (int index = 0; index < Total_tiles; index++){

            pair<int, int> PieceCoords = PieceCoordinates(index);

            Color squareColor = (PieceCoords.first + PieceCoords.second) % 2 == 0 ? lightSquare : darkSquare;

            int LocationX = BoardOffsetX + (abs((isBoardReversed * ReverseOffset) - PieceCoords.second)) * tileSize;
            int LocationY = BoardOffsetY + (abs((isBoardReversed * ReverseOffset) - PieceCoords.first)) * tileSize;

            DrawRectangle(LocationX, LocationY, tileSize, tileSize, squareColor);

            // if (!FinalIndexes.isEmpty() && FinalIndexes.peek() == index) {
            //     DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

            //}
            // else if (FinalIndexes.CompareIndex(index) && FinalIndexes.peek() != index) {
            //    DrawRectangle(LocationX, LocationY, tileSize, tileSize, MovesForPieceHighLight);

            //}

            // Highlight Last Move
            if (MoveIndices.first == index || MoveIndices.second == index){
                DrawRectangle(LocationX, LocationY, tileSize, tileSize, MoveHighlight);
            }
            // Show Moves for Piece
            for (const auto& move : MovesForSelectedPiece){
                if (move == index) {
                    float Diameter = 26;
                    float Radius = Diameter / 2;
                    float Offset = Diameter + Radius;

                    Rectangle CorrectMoves = { static_cast<float>(LocationX) + Offset, static_cast<float>(LocationY) + Offset, Diameter, Diameter };
                    DrawCircle(LocationX + static_cast<int>(Offset), LocationY + static_cast<int>(Offset), Radius, Translucent);
                }
            }
        }
    }

    void DrawChessPiece() const {
        for (int index = 0; index < Total_tiles; index++){

            int PositionX = static_cast<int>(board[index].rectangle.x);
            int PositionY = static_cast<int>(board[index].rectangle.y);
            DrawTexture(board[index].texture, PositionX, PositionY, WHITE);
        }
    }

    int getTileIndex(float x, float y, int tileSize) {
        
        int file = static_cast<int>(x / tileSize);
        int rank = static_cast<int>(y / tileSize);
        int index = rank * 8 + file;

        return index;
    }

    void UpdateChessPiece(ChessPiece& piece, int InitialIndex){
        if (piece.type != EMPTY){

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rectangle)){
                piece.isDragged = true;
                MovesForSelectedPiece = GetAllPossibleMovesForPiece(piece.type, InitialIndex);
                // DisplayMoves(MovesForSelectedPiece);
                // cout << InitialIndex << endl;
            }
            if (piece.isDragged){
                piece.rectangle.x = GetMouseX() - piece.rectangle.width / 2;
                piece.rectangle.y = GetMouseY() - piece.rectangle.height / 2;

                //Prevent Pieces from being Dragged out of the Screen
                if (piece.rectangle.x < 0) piece.rectangle.x = 0;
                if (piece.rectangle.y < 0) piece.rectangle.y = 0;
                if (piece.rectangle.x + piece.rectangle.width > GetScreenWidth()) piece.rectangle.x = GetScreenWidth() - piece.rectangle.width;
                if (piece.rectangle.y + piece.rectangle.height > GetScreenHeight()) piece.rectangle.y = GetScreenHeight() - piece.rectangle.height;

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    float tileX = round((piece.rectangle.x - BoardOffsetX) / tileSize) * tileSize;
                    float tileY = round((piece.rectangle.y - BoardOffsetY) / tileSize) * tileSize;
                    int FinalIndex = abs((isBoardReversed * (Total_tiles - 1)) - getTileIndex(tileX, tileY, tileSize));
                    InitialIndex = abs((isBoardReversed * ReverseOffset) - InitialIndex); // adjust for Reversals
                    string Move = ConvertToChessNotation(InitialIndex, FinalIndex);

                    // cout << move << endl;
                    // cout << InitialIndex << " " << FinalIndex << endl;

                    // cout << tileX << " " << tileY << endl;
                    if (isValidMove(FinalIndex)){
                        piece.isDragged = false;
                        piece.rectangle.x = tileX;
                        piece.rectangle.y = tileY;

                        MakeCompleteMove(InitialIndex, FinalIndex, Move);
                        DisplayBoard();
                        // WaitTime(0.5);
                    }
                    else{
                        piece.isDragged = false;
                        int file = abs((isBoardReversed * ReverseOffset) - (InitialIndex % 8));
                        int rank = abs((isBoardReversed * ReverseOffset) - (InitialIndex / 8));
                        piece.rectangle.x = BoardOffsetX + static_cast<float>(file * tileSize);
                        piece.rectangle.y = BoardOffsetY + static_cast<float>(rank * tileSize);
                    }
                }
            }
        }
    }

    bool isValidMove(int index)
    {
        for (auto Move : MovesForSelectedPiece)
        {
            if (index == Move)
            {
                return true;
            }
        }
        return false;
    }

    // void MakeMove(int fromTile, int toTile)
    //{
    //     string move = ConvertToChessNotation(fromTile, toTile);
    //     // Check if it's a castling move and if castling is allowed
    //     if (IsCastlingMove(move, board[fromTile]))
    //     {
    //         if (toTile == 62 || toTile == 7)
    //         { // Kingside castling
    //             int rookFromTile = (fromTile == 60) ? 63 : 7;
    //             int rookToTile = (fromTile == 60) ? 61 : 5;
    //             board[rookToTile] = board[rookFromTile];
    //             board[rookFromTile] = ChessPiece();
    //         }
    //         else if (toTile == 58 || toTile == 2)
    //         { // Queenside castling
    //             int rookFromTile = (fromTile == 60) ? 56 : 0;
    //             int rookToTile = (fromTile == 60) ? 59 : 3;
    //             board[rookToTile] = board[rookFromTile]; // Move the rook
    //             board[rookFromTile] = ChessPiece();      // Empty the original rook square
    //         }
    //     }
    //     board[fromTile].firstMove = false;
    //     board[toTile] = board[fromTile];
    //     board[fromTile] = ChessPiece(); // Empty the source square
    // }
    // bool IsCastlingMove(string move, ChessPiece pieceMoved)
    //{
    //     return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || "e8g8" || move == "e8c8");
    // }

    void MakeMove(int fromTile, int toTile){
        string move = ConvertToChessNotation(fromTile, toTile);

        // Check if it's a castling move and if castling is allowed
        if (IsCastlingMove(move, board[fromTile])){

            if (toTile == 62 || toTile == 7){ // Kingside castling
                int rookFromTile = (fromTile == 60) ? 63 : 7;
                int rookToTile = (fromTile == 60) ? 61 : 5;
                board[rookFromTile].firstMove = false;
                board[rookToTile] = board[rookFromTile];
                board[rookFromTile] = ChessPiece();
                

            }
            else if (toTile == 58 || toTile == 2){ // Queenside castling
                int rookFromTile = (fromTile == 60) ? 56 : 0;
                int rookToTile = (fromTile == 60) ? 59 : 3;
                board[rookFromTile].firstMove = false;
                board[rookToTile] = board[rookFromTile]; // Move the rook
                board[rookFromTile] = ChessPiece();      // Empty the original rook square
            }
        }

        board[fromTile].firstMove = false;
        board[toTile] = board[fromTile];
        board[fromTile] = ChessPiece(); // Empty the source square
        MovesForSelectedPiece.clear();
        
        if(flags.isMultiplayerGame()) ReverseBoard();
    }

    void ReverseBoard() {
        isBoardReversed = !isBoardReversed;

    }

    bool IsCastlingMove(string move, ChessPiece pieceMoved)
    {

        return (pieceMoved.type == KING) && (move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8");
    }

     void MakeCompleteMove(int fromTile, int toTile, string move){
        // string move = ConvertToChessNotation(Move);
        PieceIsCaptured = isValidCaptureMove(fromTile, toTile);
        MakeMove(fromTile, toTile);

        // Check for pawn promotion
        if (board[toTile].type == PAWN){
            // White pawn promotion
            if (toTile >= 0 && toTile <= 7){
                promotePawn(toTile);
            }
            // Black pawn promotion
            else if (toTile >= 56 && toTile <= 63){
                cout << "Test";
                promotePawn(toTile);
            }
        }

        cout << (currentPlayerIsWhite ? "White" : "Black") << " Plays : " << move << "\n";

        if (moveHistory.size() >= MaxHistorySize){
            moveHistory.clear();
        }


        flags.CheckGameState();
        AddMoveToHistory(move);
        MoveIndices = make_pair(fromTile, toTile);

        SetPiecePositions();

        currentPlayerIsWhite = !currentPlayerIsWhite;
        ComputeOpponentMoves();
        PlayChessSound();
        MovesForSelectedPiece.clear();
    }

    void PlayChessSound() const{
        ChessBoard temp = *this;
        int PlayerColor = (temp.isCurrentPlayerWhite()) ? White : Black;
        if (isCheck(temp, PlayerColor, "board: Play Chess Sound")){
            PlaySound(KingChecked);
        }
        else if (PieceIsCaptured) {
            PlaySound(ChessPieceCaptured);
        }
        else {
            PlaySound(ChessPiecePlaced);
        }
    }

    bool isCurrentPlayerWhite() const
    {
        return currentPlayerIsWhite;
    }

    void DisplayScores() const{
        int whiteScore = calculatePlayerScore(White);
        int blackScore = calculatePlayerScore(Black);
        cout << "\nWhite score: " << whiteScore << " | Black score: " << blackScore << "\n\n";
    }

    void ComputeSlidingPieceMoves(int pieceIndex, vector<int>& possibleMoves) const{
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Offsets for sliding directions: up, down, left, right, diagonals
        vector<int> offsets = { 8, -8, 1, -1, 7, -7, 9, -9 };

        if (piece.type == ROOK || piece.type == QUEEN) {
            for (int i = 0; i < 4; ++i){
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < Total_tiles){
                    // Check if the new index is within the bounds of the board
                    if ((offset == 1 || offset == -1) && newIndex / 8 != row)
                        break; // If moving horizontally, ensure it stays on the same row
                    if (board[newIndex].type != EMPTY){
                        if (board[newIndex].color != piece.color){
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                            possibleMoves.push_back(newIndex);
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(newIndex);
                    newIndex += offset;
                }
            }
        }

        if (piece.type == BISHOP || piece.type == QUEEN) {
            for (int i = 4; i < 8; ++i){
                int offset = offsets[i];
                int newIndex = pieceIndex + offset;
                while (newIndex >= 0 && newIndex < Total_tiles){
                    // Check if the new index is within the bounds of the board
                    int newRow = newIndex / 8;
                    int newCol = newIndex % 8;
                    if (abs(newRow - row) != abs(newCol - col))
                        break; // If not moving diagonally, break
                    if (board[newIndex].type != EMPTY){
                        if (board[newIndex].color != piece.color){
                            // If the square contains an enemy piece, add it to possibleMoves and stop searching in this direction
                            possibleMoves.push_back(newIndex);
                        }
                        break; // Stop searching in this direction if we encounter a piece
                    }
                    possibleMoves.push_back(newIndex);
                    newIndex += offset;
                }
            }
        }
    }

    void ComputeKnightMoves(int pieceIndex, vector<int>& possibleMoves) const {
        ChessPiece piece = board[pieceIndex];
        // Offsets for knight's moves
        vector<int> rowOffsets = { -2, -1, 1, 2, 2, 1, -1, -2 };
        vector<int> colOffsets = { 1, 2, 2, 1, -1, -2, -2, -1 };

        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        if (piece.type == KNIGHT){
            for (int i = 0; i < 8; ++i) {
                int newRow = row + rowOffsets[i];
                int newCol = col + colOffsets[i];

                // Check if the new position is within the board bounds
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8){
                    int newIndex = newRow * 8 + newCol;
                    // Check if the square is empty or contains an opponent's piece
                    if (board[newIndex].type == EMPTY || board[newIndex].color != piece.color){
                        possibleMoves.push_back(newIndex);
                    }
                }
            }
        }
    }

    void ComputePawnMoves(int pieceIndex, vector<int>& possibleMoves) const{
        ChessPiece piece = board[pieceIndex];
        int row = pieceIndex / 8;
        int col = pieceIndex % 8;

        // Determine the direction of pawn movement based on its color
        int direction = (piece.color == White) ? -1 : 1;

        // Forward movement
        int forwardOne = pieceIndex + direction * 8;
        if (forwardOne >= 0 && forwardOne < Total_tiles && board[forwardOne].type == EMPTY){
            possibleMoves.push_back(forwardOne);
            // Check if the pawn is in its starting position and can move two squares forward
            if ((row == 6 && piece.color == White) || (row == 1 && piece.color == Black)){
                int forwardTwo = forwardOne + direction * 8;
                if (board[forwardTwo].type == EMPTY){
                    possibleMoves.push_back(forwardTwo);
                }
            }
        }

        // Capture moves
        vector<int> captureOffsets = { direction * 7, direction * 9 };
        for (int offset : captureOffsets){
            int targetIndex = pieceIndex + offset;
            int targetRow = targetIndex / 8;
            int targetCol = targetIndex % 8;
            int pieceCol = pieceIndex % 8;
            // Ensure target column is within bounds and not wrapping around the board
            if (targetCol >= 0 && targetCol < 8 && abs(targetCol - pieceCol) <= 1){
                // Check if the target square is occupied by an opponent's piece
                if (board[targetIndex].type != EMPTY && board[targetIndex].color != piece.color){
                    possibleMoves.push_back(targetIndex);
                }
            }
        }
    }

    void ComputeKingMoves(int KingIndex, vector<int>& possibleMoves) const {
        
        ChessPiece King = board[KingIndex];
        int row = KingIndex / 8;
        int col = KingIndex % 8;

        // Offsets for king's moves
        vector<int> rowOffsets = { -1, -1, -1, 0, 0, 1, 1, 1 };
        vector<int> colOffsets = { -1, 0, 1, -1, 1, -1, 0, 1 };

        // Check all 8 directions
        for (int i = 0; i < 8; i++){
            int newRow = row + rowOffsets[i];
            int newCol = col + colOffsets[i];

            // Check if the new position is within the board bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8){
                int newIndex = newRow * 8 + newCol;
                // Check if the square is empty or contains an opponent's piece
                if ((board[newIndex].type == EMPTY || board[newIndex].color != King.color)){
                    possibleMoves.push_back(newIndex);
                }
            }
        }

        if ((King.color == Black && KingIndex == 4) || (King.color == White && KingIndex == 60)){

            // Check for castling moves
            if (King.firstMove && !IsTileUnderAttack(KingIndex)){
                // Check kingside castling
                if (canCastleKingSide(KingIndex)){    
                    
                    possibleMoves.push_back(KingIndex + 2);
                }
                // Check queenside castling
                if (canCastleQueenide(KingIndex)){
                    //cout << KingIndex << endl;
                    possibleMoves.push_back(KingIndex - 2);
                }
            }
        }
    }

    bool canCastleKingSide(int KingIndex) const{
        if (!board[KingIndex].canCastleKingSide){
            return false;
        }
        
        int RookIndex = KingIndex + 3;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)){
            return false;
        }
        
        for (int i = KingIndex + 1; i < RookIndex; i++){

            if (board[i].type != EMPTY || IsTileUnderAttack(i)){
                
                return false;
            }
            
        }
        
        return true;
    }

    bool canCastleQueenide(int KingIndex) const{
        if (!board[KingIndex].canCastleQueenSide){
            return false;
        }

        int RookIndex = KingIndex - 4;
        if (board[RookIndex].type != ROOK && !(board[RookIndex].firstMove)){
            return false;
        }

        for (int i = KingIndex - 1; i > RookIndex; i--){
            if (board[i].type != EMPTY || IsTileUnderAttack(i)){
                return false;
            }
        }

        return true;
    }

    bool isCheck(const ChessBoard& chessboard, int playerColor, string calledby) const{

        //cout << "Being Called by: " << calledby << endl;
        // Find the king's position
        int kingIndex = -100;
        for (int i = 0; i < Total_tiles; i++){
            if (chessboard.board[i].type == KING && chessboard.board[i].color == playerColor){
                kingIndex = i;
                break;
            }
        }

        if (kingIndex == -100){
            cout << "\nError Code 404: King not found. \n\n";
            cout << calledby << "\n\n";
            exit(404);
            return false;
        }

        // Iterate through opponent's pieces and see if any can attack the king
        int opponentColor = (playerColor == White) ? Black : White;
        vector<int> possibleMoves;
        for (int i = 0; i < Total_tiles; i++){
            if (chessboard.board[i].color == opponentColor){
                if (chessboard.board[i].isSlidingPiece){           
                    ComputeSlidingPieceMoves(i, possibleMoves); // Example function, change as needed
                }
                else if (chessboard.board[i].type == PAWN){
                    ComputePawnMoves(i, possibleMoves);
                }
                else if (chessboard.board[i].type == KNIGHT){
                    ComputeKnightMoves(i, possibleMoves);
                }
                else if (chessboard.board[i].type == KING){
                    ComputeKingMoves(i, possibleMoves);
                }

                // Check if any of the possible moves include the king's position
                for (int move : possibleMoves){
                    if (move == kingIndex){
                        // PlaySound(KingChecked);
                        return true; // King is in check
                    }
                }
            }
        }
        return false; // King is not in check
    }

    bool isCheckmate(ChessBoard& chessboard, int playerColor) const{
        if (!isCheck(chessboard, playerColor, "board: Is Check Mate")){
            return false; // King is not in check, so it can't be checkmate
        }

        vector<int> possibleMoves = GetAllPossibleMoves(playerColor);
        if (possibleMoves.size() > 0){
            return false; // King can escape from check, so it's not checkmate
        }

        return true; // King is in checkmate
    }

    vector<int> GetAllPossibleMoves(int playerColor) const{

        vector<int> possibleMoves;

        for (int i = 0; i < Total_tiles; i++){    
            if (board[i].color == playerColor){
                vector<int> moves;

                // Compute moves based on the piece type
                if (board[i].isSlidingPiece){            
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (board[i].type == PAWN){
                    ComputePawnMoves(i, moves);
                }
                else if (board[i].type == KNIGHT){
                    ComputeKnightMoves(i, moves);
                }
                else if (board[i].type == KING){
                    ComputeKingMoves(i, moves);
                }

                moves = FilterValidMoves(i, moves);
                for (auto move : moves){
                    possibleMoves.push_back(move);
                }
            }
        }

        return possibleMoves;
    }

    vector<string> GetAllPossibleMovesInChessNotation(int playerColor) const{
        vector<string> possibleMoves;

        for (int i = 0; i < Total_tiles; i++){
            if (board[i].color == playerColor){
                vector<int> moves;

                // Compute moves based on the piece type
                if (board[i].isSlidingPiece){
                    ComputeSlidingPieceMoves(i, moves);
                }
                else if (board[i].type == PAWN){
                    ComputePawnMoves(i, moves);
                }
                else if (board[i].type == KNIGHT){
                    ComputeKnightMoves(i, moves);
                }
                else if (board[i].type == KING){
                    ComputeKingMoves(i, moves);
                }

                // Check if the move is legal (does not leave the king in check)
                for (int toTile : moves){
                    // Simulate the move
                    ChessBoard tempBoard = *this;
                    tempBoard.MakeMove(i, toTile);

                    // Check if the king is in check after the move
                    if (!tempBoard.isCheck(tempBoard, playerColor, "board: Get all possible moves in chess notation")){
                        // If not in check, add the move to possible moves
                        possibleMoves.push_back(ConvertToChessNotation(i, toTile));
                    }
                }
            }
        }

        return possibleMoves;
    }

    string ConvertToChessNotation(int fromTile, int toTile) const{
        string chessNotation = "";

        // Convert the 'fromTile' index to chess notation
        char fromCol = 'a' + (fromTile % 8);
        int fromRow = 8 - (fromTile / 8);
        chessNotation += fromCol;
        chessNotation += to_string(fromRow);

        // Add the 'toTile' index to chess notation
        char toCol = 'a' + (toTile % 8);
        int toRow = 8 - (toTile / 8);
        chessNotation += toCol;
        chessNotation += to_string(toRow);

        return chessNotation;
    }

    int getAttacksOnSquare(int squareIndex, int opponentColor) const{
        int attacks = 0;
        vector<int> opponentMoves = GetAllPossibleMoves(opponentColor);
        for (int i = 0; i < opponentMoves.size(); ++i){
            if (opponentMoves[i] == squareIndex){
                attacks++;
            }
        }
        return attacks;
    }

    bool IsTileUnderAttack(int squareIndex) const{

        for (int i = 0; i < OpponentMoves.size(); i++){
            if (OpponentMoves[i] == squareIndex){
                return true;
            }
        }
        return false;
    }

    void DisplayMoves(){
        for (auto move : OpponentMoves){
            cout << move << endl;
        }
    }

    vector<int> GetAllPossibleMovesForPiece(int type, int index) const{
        vector<int> moves;

        // Compute moves based on the piece type
        if (type == ROOK || type == BISHOP || type == QUEEN){
            ComputeSlidingPieceMoves(index, moves);
        }
        else if (type == PAWN){
            ComputePawnMoves(index, moves);
        }
        else if (type == KNIGHT){
            ComputeKnightMoves(index, moves);
        }
        else if (type == KING){
            ComputeKingMoves(index, moves);
        }

        moves = FilterValidMoves(index, moves);
        return moves;
    }

    void DestroyBoard(){
        for (int i = 0; i < Total_tiles; i++){
            board[i].DestroyTextures();
        }
    }

    ChessPiece GetPieceAtPosition(int index) const{
        return board[index];
    }

    vector<string> GetAllCaptureMovesInChessNotation(int color) const{
        vector<string> captureMoves;

        for (int fromTile = 0; fromTile < Total_tiles; fromTile++){
            ChessPiece piece = board[fromTile];
            if (piece.color == color){
                for (int toTile = 0; toTile < Total_tiles; toTile++){
                    if (fromTile != toTile && isValidCaptureMove(fromTile, toTile)){
                        string move = ConvertToChessNotation(fromTile, toTile);
                        captureMoves.push_back(move);
                    }
                }
            }
        }

        return captureMoves;
    }

    bool isValidCaptureMove(int fromTile, int toTile) const{
        ChessPiece fromPiece = board[fromTile];
        ChessPiece toPiece = board[toTile];

        // Check if there is a piece at the 'from' tile and it is of opposite color to the piece at the 'to' tile
        return fromPiece.type != EMPTY && toPiece.type != EMPTY && fromPiece.color != toPiece.color;
    }

    pair<int, int> convertChessNotationToIndices(const string& move) const{
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void ComputeOpponentMoves(){
        OpponentMoves = GetAllPossibleMoves(isCurrentPlayerWhite() ? Black : White);
    }
};


// Define a structure for a transposition table entry
class TranspositionTables {

    uint64_t zobristKeys[64][12]; // 64 squares, 12 piece types

    unordered_map<uint64_t, pair<float, int>> transpositionTable; // <hash, <score, depth>>


public:
    double SizeOfTranspostionTable;

    TranspositionTables() {
        SizeOfTranspostionTable = 0.0;

        initZobristKeys();
    }

    void initZobristKeys() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 12; ++j) {
                zobristKeys[i][j] = dis(gen);

            }
        }
    }

    // Compute hash value for the current board position
    uint64_t computeHash(const ChessBoard& board) const {
        uint64_t hash = 0;
        for (int i = 0; i < 64; ++i) {
            ChessPiece piece = board.board[i];
            if (piece.type != EMPTY) {
                int index = piece.PieceCode;
                hash ^= zobristKeys[i][index];
            }
        }
        return hash;
    }


    // Search in transposition table
    pair<float, int> lookupTranspositionTable(uint64_t hash) const {
        auto it = transpositionTable.find(hash);
        if (it != transpositionTable.end()) {
            return it->second;
        }
        return { -INFINITY, -1 }; // Not found
    }

    // Store in transposition table
    void storeTranspositionTable(uint64_t hash, float score, int depth) {
        transpositionTable[hash] = { score, depth };
        ComputeSizeOfTranspositionTable();
    }

    void ComputeSizeOfTranspositionTable() {

        size_t pairSize = sizeof(pair<const uint64_t, pair<float, int>>);                        // Size of each key-value pair in bytes
        size_t numPairs = transpositionTable.size();                                             // Total number of pairs in the map
        size_t totalPairSize = pairSize * numPairs;                                              // Total size of pairs in bytes
        size_t overhead = sizeof(transpositionTable);                                            // Overhead of the unordered_map itself
        SizeOfTranspostionTable = static_cast<double>(totalPairSize + overhead) / (1024 * 1024); // Convert bytes to megabytes


    }

    bool isValuePresent(uint64_t hash) {

        for (const auto& key : transpositionTable) {
            if (key.first == hash) {
                return true;
            }
        }
        return false;
    }

    //~TranspositionTables(){
    //    for (int i = 0; i < 64; ++i) {
    //        delete[] zobristKeys[i];
    //    }
    //    delete[] zobristKeys;

    //}

};




class ChessEngine : public ReadWrite{
    int color;
    int MAX_DEPTH = 3; // Maximum depth for the Minimax algorithm
    const int infinity = numeric_limits<int>::max();
    TranspositionTables transpostionTable;

    bool terminateSearch; // Atomic flag variable to terminate the search
    bool startSearch; // Atomic flag variable to start the search

    // Piece-square tables for each piece type
    // Higher values indicate a more advantageous position for the piece
    const int PawnPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };
    const int RooksPST[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
    };
    const int KnightsPST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };
    const int BishopsPST[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };
    const int QueensPST[64] = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };
    const int KingPST[64] = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };
    const int KingEndgamePST[64] = {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };

public:
    int engineEloRating = 500;
    int NumberofMovesLookedAhead;
    long long TimeTakenForSearch;
    int NumberOfBranchesPruned;

    ChessEngine(int Color = EMPTY) : color(Color){
        engineEloRating = readEloFromFile();
        NumberofMovesLookedAhead = 0;
        TimeTakenForSearch = 0;
        NumberOfBranchesPruned = 0;
        terminateSearch = false;
        startSearch = false;
    }

    void setEngineColor(int color) {
        this->color = color;
    }

    void shuffleMoves(vector<string>& possibleMoves){
        long long seed = system_clock::now().time_since_epoch().count();
        shuffle(possibleMoves.begin(), possibleMoves.end(), default_random_engine(seed));
    }

    void SetDepth(int NewDepth) {
        MAX_DEPTH = NewDepth;
    }

    string GenerateMove(const ChessBoard& board){
        int bestScore = -infinity;
        string bestMove;
        auto start = high_resolution_clock::now();
        NumberofMovesLookedAhead = 0;
        vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
        //DisplayMoves(possibleMoves);
        
        //SortMoves(possibleMoves, board, color);
        //DisplayMoves(possibleMoves);

        shuffleMoves(possibleMoves);

        for (const string& move : possibleMoves){
            if (terminateSearch) {
                cout << "xxxxxxxxxx Search Terminated xxxxxxxxxx" << endl;
                terminateSearch = false;
                break;
            }


            ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
            //int fromTile, toTile;
            // tempBoard.isValidMove(move, fromTile, toTile);
            pair<int, int> Indices = convertChessNotationToIndices(move);

            // Simulate the move
            tempBoard.MakeMove(Indices.first, Indices.second);

            // Calculate the score using Minimax with alpha-beta pruning
            int score = Minimax(tempBoard, MAX_DEPTH, -infinity, infinity, false, color, NumberofMovesLookedAhead);

            // Update the best move if needed
            if (score >= bestScore){
                bestScore = score;
                bestMove = move;
            }
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(end - start);
            TimeTakenForSearch = duration.count();
        }

        possibleMoves.clear();

        return bestMove;
    }

    int Minimax(ChessBoard& board, int depth, int alpha, int beta, bool maximizingPlayer, int color, int& MovesLookedAhead){
        MovesLookedAhead++;


        uint64_t hash = transpostionTable.computeHash(board);
        
    
        if (transpostionTable.isValuePresent(hash)) {

            auto stored = transpostionTable.lookupTranspositionTable(hash);
            if (stored.second >= depth) {
                //cout << hash << endl;
                return stored.first; // Return the stored evaluation if it's at least as deep as the current search depth
            }
        }


        // Base case: reached maximum depth or terminal state (checkmate)
        if (depth == 0 || board.isCheckmate(board, color)){
            if (board.isCheckmate(board, color)){
                return (maximizingPlayer ? -infinity : infinity); // Checkmate score
            }
            return Evaluate(board, color);
        }

        // Maximizing player's turn
        if (maximizingPlayer){
            int maxScore = -infinity;
            vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation(color);
            shuffleMoves(possibleMoves);
            //SortMoves(possibleMoves, board, color);

            for (const string& move : possibleMoves){
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, false, color, MovesLookedAhead);

                // Update alpha and maxScore
                maxScore = max(maxScore, score);
                alpha = max(alpha, score);

                //Store Table
                transpostionTable.storeTranspositionTable(hash, maxScore, depth);

                //Alpha-beta pruning
                if (beta <= alpha){
                    NumberOfBranchesPruned++;
                    break;
                }
            }
            possibleMoves.clear();
            return maxScore;
        }
        // Minimizing player's turn
        else{
            int minScore = infinity;
            vector<string> possibleMoves = board.GetAllPossibleMovesInChessNotation((color == White ? Black : White));
            shuffleMoves(possibleMoves);
            //SortMoves(possibleMoves, board, color == White ? Black : White);

            for (const string& move : possibleMoves){
                ChessBoard tempBoard = board; // Make a copy of the board to simulate moves
                pair<int, int> Indices = convertChessNotationToIndices(move);
                tempBoard.MakeMove(Indices.first, Indices.second); // Make the move directly without isValidMove()

                // Recursive call for the opponent with a reduced depth
                int score = Minimax(tempBoard, depth - 1, alpha, beta, true, color, MovesLookedAhead);

                // Update beta and minScore
                minScore = min(minScore, score);
                beta = min(beta, score);

                //Store Table
                transpostionTable.storeTranspositionTable(hash, minScore, depth);

                //Alpha-beta pruning
                if (beta <= alpha){
                    NumberOfBranchesPruned++;
                    break;
                }
            }
            possibleMoves.clear();
            return minScore;
        }
    }

    int Evaluate(const ChessBoard& chessboard, char currentPlayerColor) const{
        unordered_map<char, int> pieceValues;
        // Piece values for evaluation
        if (color == White){
            pieceValues = {
                {White | PAWN,  100}, {White | KNIGHT,  320}, {White | BISHOP,  330}, {White | ROOK,  500}, {White | QUEEN,  900}, {White | KING,  infinity},
                {Black | PAWN, -100}, {Black | KNIGHT, -320}, {Black | BISHOP, -330}, {Black | ROOK, -500}, {Black | QUEEN, -900}, {Black | KING, -infinity} };
        }
        else{
            pieceValues = {
                {White | PAWN, -100}, {White | KNIGHT, -320}, {White | BISHOP, -330}, {White | ROOK, -500}, {White | QUEEN, -900}, {White | KING, -infinity},
                {Black | PAWN,  100}, {Black | KNIGHT,  320}, {Black | BISHOP,  330}, {Black | ROOK,  500}, {Black | QUEEN,  900}, {Black | KING,  infinity} };
        }

        // Evaluate material advantage
        int SelfMaterial = 0, OpponentMeterial = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                if (piece.color == color)
                {
                    SelfMaterial += pieceValues[piece.PieceCode];
                }
                else
                {
                    OpponentMeterial += pieceValues[piece.PieceCode];
                }
            }
        }
        int materialAdvantage = (currentPlayerColor == White) ? (SelfMaterial - OpponentMeterial) : (OpponentMeterial - SelfMaterial);

        // Evaluate positional advantage using PSTs
        int positionalAdvantage = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                int pieceValue = getPSTValue(piece, i, currentPlayerColor);
                positionalAdvantage += pieceValue;
            }
        }

        // Total evaluation is a combination of material, positional, and safety advantages
        return materialAdvantage + positionalAdvantage;
    }

    int getPSTValue(ChessPiece piece, int squareIndex, char currentPlayerColor) const
    {
        const int* piecePST = nullptr;

        switch (piece.type)
        {
        case PAWN:
            piecePST = (currentPlayerColor == Black) ? InvertTable(PawnPST) : PawnPST;
            break;
        case KNIGHT:
            piecePST = (currentPlayerColor == Black) ? InvertTable(KnightsPST) : KnightsPST;
            break;
        case BISHOP:
            piecePST = (currentPlayerColor == Black) ? InvertTable(BishopsPST) : BishopsPST;
            break;
        case ROOK:
            piecePST = (currentPlayerColor == Black) ? InvertTable(RooksPST) : RooksPST;
            break;
        case QUEEN:
            piecePST = (currentPlayerColor == Black) ? InvertTable(QueensPST) : QueensPST;
            break;
        case KING:
            piecePST = (currentPlayerColor == Black) ? InvertTable(KingPST) : KingPST;
            break;
        }

        // Ensure that the piece type index is within bounds
        if (piecePST != nullptr)
        {
            return piecePST[squareIndex];
        }
        else
        {
            // Handle unknown piece types or other errors
            return 0; // Or handle the error in an appropriate way
        }
    }

    pair<int, int> convertChessNotationToIndices(const string& move) const
    {
        int fromCol = move[0] - 'a';
        int fromRow = 7 - (move[1] - '1');
        int toCol = move[2] - 'a';
        int toRow = 7 - (move[3] - '1');
        int fromTile = fromRow * 8 + fromCol;
        int toTile = toRow * 8 + toCol;
        return make_pair(fromTile, toTile);
    }

    void PlayMove(const string& move, ChessBoard& board) const{

        pair<int, int> Indices = convertChessNotationToIndices(move);
        board.MakeCompleteMove(Indices.first, Indices.second, move);
    }

    int* InvertTable(const int* originalArray) const
    {
        int* invertedArray = new int[Total_tiles]; // Create an array of size 64

        for (int i = 0; i < Total_tiles; i++)
        {
            invertedArray[i] = originalArray[63 - i];
        }

        return invertedArray;
    }

    int readEloFromFile(){
        ifstream inputFile;
        string filename = "Engine'sELO.txt";
        inputFile.open(filename);

        int elo = engineEloRating; // Default value in case the file cannot be opened

        if (inputFile.is_open()){
            inputFile >> elo;
            inputFile.close();
            cout << "Elo rating read from " << filename << ": " << elo << endl;
        }
        else{
            cout << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;

        }

        return elo;
    }

    void saveEloToFile() const{
        ofstream outputFile;
        string filename = "Engine'sELO.txt";
        outputFile.open(filename);

        int defaultElo = 500;

        if (outputFile.is_open()){

            outputFile << engineEloRating << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else{
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open())            {
                outputFile << defaultElo << std::endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else{
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }

    void Reset(){
        TimeTakenForSearch = 0;
        NumberofMovesLookedAhead = 0;
        //terminateSearch = false;
        startSearch = false;
        NumberOfBranchesPruned = 0;
    }

    // Function to check if a move results in a check
    bool CheckAfterMove(const string& move, const ChessBoard& board, char color) const
    {
        ChessBoard tempBoard = board; // Make a copy of the board to simulate the move
        pair<int, int> indices = convertChessNotationToIndices(move);
        tempBoard.MakeMove(indices.first, indices.second); // Make the move directly without isValidMove()

        // Check if the move puts the opponent's king in check
        return tempBoard.isCheck(tempBoard, (color == White ? Black : White), "Engine: Check after move");
    }

    bool IsCaptureMove(const string& move, const ChessBoard board) const
    {
        pair<int, int> indices = convertChessNotationToIndices(move);
        ChessPiece targetPiece = board.GetPieceAtPosition(indices.second);

        // Check if the target piece is an opponent's piece
        return targetPiece.color != board.GetPieceAtPosition(indices.first).color && targetPiece.color != EMPTY;
    }

    // Function to sort moves considering checks and captures first
    void SortMoves(vector<string>& moves, const ChessBoard& board, int color) const
    {
        // Define custom comparator to sort moves
        auto customComparator = [&](const string& move1, const string& move2)
            {
                // Check if move1 gives check
                bool check1 = CheckAfterMove(move1, board, color);

                // Check if move2 gives check
                bool check2 = CheckAfterMove(move2, board, color);

                // If one move gives check and the other doesn't, prioritize the move that gives check
                if (check1 != check2)
                    return check1;

                // If engineh moves give check or engineh moves don't give check, prioritize captures
                bool capture1 = IsCaptureMove(move1, board);
                bool capture2 = IsCaptureMove(move2, board);

                return capture1 > capture2; // Sorting captures before non-captures
            };

        // Sort moves using the custom comparator
        sort(moves.begin(), moves.end(), customComparator);
    }

    double getSizeOfTranspositionTable() const {
        return transpostionTable.SizeOfTranspostionTable;
    }

    void StartSearch(){
        
        startSearch = true;
    }

    void StopSearching() {
        startSearch = false; // Set the stop flag to stop the search
    }

    void TerminateSearch() {
        terminateSearch = true;
    }

    bool isSearchStarted() const{ 

        return startSearch; 
    }

    void DisplayMoves(vector<string>& moves) {
        for (const auto& move : moves) {
            cout << move << " ";
        }
        cout << endl;
    }

};

class User : public ReadWrite{
public:
    string Username;
    int ELO = 500;

    User(string name = "") : Username(name) {
        ELO = readEloFromFile();
    }

    void setUserName(string name) {
        Username = name;

    }

    int readEloFromFile(){
        cout << "TEst" << endl;
        ifstream inputFile;
        string filename = Username + "ELO.txt";
        inputFile.open(filename);

        int elo = ELO; // Default value in case the file cannot be opened

        if (inputFile.is_open()){
            inputFile >> elo;
            inputFile.close();
            cout << "Elo rating read from " << filename << ": " << elo << endl;
        }
        else{
            cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;
        }

        return elo;
    }

    void saveEloToFile() const{
        ofstream outputFile;
        string filename = Username + "ELO.txt";
        outputFile.open(filename);

        int defaultElo = 500;

        if (outputFile.is_open())
        {

            outputFile << ELO << endl;
            outputFile.close();
            cout << "Elo rating saved to " << filename << endl;
        }
        else
        {
            cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
            outputFile.open(filename);
            if (outputFile.is_open())
            {
                outputFile << defaultElo << std::endl;
                outputFile.close();
                cout << "New file created with default Elo rating: " << defaultElo << endl;
            }
            else
            {
                cerr << "Unable to create file " << filename << " for writing." << endl;
            }
        }
    }
};

class BoardStats{
private:
    const float TextSize = 50;

    const float textX = (InfoBoxWidth / 2) + InfoBoxX;
    const float textY = 100;
    const int BoardDimensions = tileSize * boardSize;
    const Color messageColor = WHITE;
    const Color AlertColor = RED;
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

    BoardStats(){
        ShowMoveHistory = false;
        winner = NULL;
        SaveData = true;
        EndMessage = "";
    }

    void DisplayEndMessage(){
        float posX = textX - ((TextCenter(EndMessage.c_str(), fontSize - 10).x) / 2);
        float posY = (static_cast<float>(screenHeight) / 2) - ((TextCenter(EndMessage.c_str(), fontSize - 10).y) / 2);

        DrawTextWithCustomFont(EndMessage.c_str(), posX, posY, fontSize - 10, AlertColor); // Balance Based on TextSize
    }

    bool GameIsEnded(ChessBoard& board){

        if(flags.MoveIsMade()){

            cout << "Checking....." << endl;
            int playerColor = board.isCurrentPlayerWhite() ? White : Black;
           
            if (board.isCheckmate(board, playerColor)) {

                //Set message to display
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
            if (board.calculatePlayerScore(White) <= 100 && board.calculatePlayerScore(Black) <= 100) {
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

    string CloseInBrackets(string stringClose){
        string temp = "(";
        temp += stringClose;
        temp += ")";
        return temp;
    }

    void DisplayStats(ChessBoard& chessboard, ChessEngine& engine, User& player){
        
        Color CheckAlertB = messageColor;
        Color CheckAlertW = messageColor;
        string LastMovePlayed = "";
        string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

        if (flags.MoveIsMade()) {
            //cout << flags.MoveIsMade << endl;
            CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? messageColor : AlertColor;
            CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? messageColor : AlertColor;
        }

        if (chessboard.moveHistory.size() != 0){
            LastMovePlayed = (chessboard.moveHistory.back());
        }


       

        MovesAndHistory(LastMovePlayed, chessboard);
        DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
        DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertB);
        // (isSinglePlayer) {
        int LookAheads = engine.NumberofMovesLookedAhead;
        long long TimeTaken = engine.TimeTakenForSearch;
        int BranchesPruned = engine.NumberOfBranchesPruned;
        double SizeOfTable = engine.getSizeOfTranspositionTable();
        string WhiteELO = CloseInBrackets(ELO + to_string(player.ELO));
        string BLackELO = CloseInBrackets(ELO + to_string(engine.engineEloRating));
        string BlackMessage = "Saw " + to_string(LookAheads) + " futures in " + to_string(TimeTaken) + "s";
        string PruningMessage = "Pruned " + to_string(BranchesPruned) + " Branches";
        string TableSizeMessage = "Size: " + to_string(SizeOfTable);
        DrawTextWithCustomFont(BlackMessage.c_str(), textX - (TextCenter(BlackMessage.c_str(), fontSize - 30).x / 2), textY + 70, fontSize - 30, messageColor);
        DrawTextWithCustomFont(TableSizeMessage.c_str(), textX - (TextCenter(TableSizeMessage.c_str(), fontSize - 30).x / 2), textY + 90, fontSize - 30, messageColor);
        DrawTextWithCustomFont(PruningMessage.c_str(), textX - (TextCenter(PruningMessage.c_str(), fontSize - 30).x / 2), textY + 110, fontSize - 30, messageColor);

        DrawTextWithCustomFont(WhiteELO.c_str(), textX - (TextCenter(WhiteELO.c_str(), fontSize - 30).x / 2), screenHeight - 146 + 30, fontSize - 30, messageColor);
        DrawTextWithCustomFont(BLackELO.c_str(), textX - (TextCenter(BLackELO.c_str(), fontSize - 30).x / 2), textY + 50, fontSize - 30, messageColor);
        //}

        // if (!chessboard.preMoves.isEmpty()) {
        //     string NextMoveToPlay = "Next in Queue: " + chessboard.preMoves.peek();
        //     DrawTextWithCustomFont(NextMoveToPlay.c_str(), textX - strlen(White_) - 20, screenHeight - 166 - fontSize + 30, fontSize - 30, messageColor);
        // }
    }

    void DisplayStats(ChessBoard& chessboard) {
        Color CheckAlertB = messageColor;
        Color CheckAlertW = messageColor;
        string LastMovePlayed = "";
        string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";

        if (flags.MoveIsMade()) {
            //cout << flags.MoveIsMade << endl;
            CheckAlertB = chessboard.isCheck(chessboard, Black, "Game stats: Display Stats") ? messageColor : AlertColor;
            CheckAlertW = chessboard.isCheck(chessboard, White, "Game stats: Display Stats") ? messageColor : AlertColor;
        }

        if (chessboard.moveHistory.size() != 0) {
            LastMovePlayed = (chessboard.moveHistory.back());
        }

        MovesAndHistory(LastMovePlayed, chessboard);
        DrawTextWithCustomFont(Black_, textX - (TextCenter(Black_, fontSize).x / 2), textY - (TextCenter(Black_, fontSize).y / 2) + 20, fontSize, CheckAlertB);
        DrawTextWithCustomFont(White_, textX - (TextCenter(White_, fontSize).x / 2), screenHeight - 146 - fontSize + 30, fontSize, CheckAlertB);
    }

    void MovesAndHistory(string LastMovePlayed, ChessBoard& chessboard){
        if (ShowMoveHistory){
            DisplayMoveHistory(chessboard);
        }
        else{
            DrawTextWithCustomFont(LastMovePlayed.c_str(), textX - (TextCenter(LastMovePlayed.c_str(), fontSize - 10).x / 2), screenHeight / 2 - 42, fontSize - 10, AlertColor);
        }
    }

    // void DisplayStats(ChessBoard chessboard, ChessEngine Black, ChessEngine White) {
    //     int LookAheadsBlack = Black.NumberofMovesLookedAhead;
    //     auto TimeTakenBlack = Black.TimeTakenForSearch;
    //     int LookAheadsWhite = White.NumberofMovesLookedAhead;
    //     auto TimeTakenWhite = White.TimeTakenForSearch;
    //     int whiteScore = chessboard.calculatePlayerScore('W');
    //     int blackScore = chessboard.calculatePlayerScore('B');
    //     string LastMovePlayed = "";
    //     string BlackMessage = "Saw " + to_string(LookAheadsBlack) + " futures in " + to_string(TimeTakenBlack) + " milliseconds";
    //     string WhiteMessage = "Saw " + to_string(LookAheadsWhite) + " futures in " + to_string(TimeTakenWhite) + " milliseconds";
    //     string currentPlayer = (chessboard.isCurrentPlayerWhite()) ? "White" : "Black";
    //     if (chessboard.moveHistory.size() != 0) {
    //         LastMovePlayed = chessboard.moveHistory.back();
    //     }
    //     //Display if Black Is in check
    //     //Display Black Score
    //     //Display Engine message
    //     //Display Last Move
    //     //Display Current Player
    //     //Display white Score
    //     //Display if White Is in Check
    //     //Black
    //     if (chessboard.isCheck(chessboard, 'B', "Stats: DisplayStats: black: 2")) {
    //         DrawTextWithCustomFont("Black King is in Check", textX, (tileSize * 1) - fontSize, fontSize, RED);
    //     }
    //     else {
    //         DrawTextWithCustomFont("Black King is not in Check", textX, (tileSize * 1) - fontSize, fontSize, BLACK);
    //     }
    //     DrawTextWithCustomFont(("Black ELO: " + to_string(Black.engineEloRating)).c_str(), textX, (tileSize * 2 - fontSize) - fontSize, fontSize, BLACK);
    //     DrawTextWithCustomFont(("Black Player Score: " + to_string(blackScore)).c_str(), textX, (tileSize * 3 - fontSize) - fontSize, fontSize, BLACK);
    //     DrawTextWithCustomFont(BlackMessage.c_str(), textX, (tileSize * 2 + fontSize) - (fontSize - 0), fontSize - 4, BLACK);
    //     //Current Player
    //     DrawTextWithCustomFont(("Current Player: " + currentPlayer).c_str(), textX, static_cast<float>(screenHeight) / 2, fontSize, (currentPlayer == "White") ? WHITE : BLACK);
    //     DrawTextWithCustomFont(LastMovePlayed.c_str(), textX, (tileSize * 5 + fontSize) - fontSize, fontSize, RED);
    //     //White
    //     DrawTextWithCustomFont(WhiteMessage.c_str(), textX, (tileSize * 7 + fontSize) - (fontSize - 0), fontSize - 4, WHITE);
    //     DrawTextWithCustomFont(("White Player Score: " + to_string(whiteScore)).c_str(), textX, (tileSize * 5 + fontSize) + fontSize, fontSize, WHITE);
    //     DrawTextWithCustomFont(("White ELO: " + to_string(White.engineEloRating)).c_str(), textX, (tileSize * 6 + fontSize) + fontSize, fontSize, WHITE);
    //     if (chessboard.isCheck(chessboard, 'W', "Stats: DisplayStats: black: 1")) {
    //         DrawTextWithCustomFont("White King is in Check", textX, (tileSize * 7) + fontSize, fontSize, RED);
    //     }
    //     else {
    //         DrawTextWithCustomFont("White King is not in Check", textX, (tileSize * 7) + fontSize, fontSize, WHITE);
    //     }
    // }

    void DisplayMoveHistory(ChessBoard chessboard)
    {

        string Moves = "";
        for (int index = 0; index < chessboard.moveHistory.size(); index++)
        {
            Moves += chessboard.moveHistory[index];
            Moves += ", ";
            if ((index + 1) % 6 == 0)
            {
                Moves += "\n\n";
            }
        }
        DrawTextWithCustomFont(Moves.c_str(), 150, textY + 90, fontSize - 20, RED);
    }

    double calculateExpectedScore(double engineRating, double opponentRating)
    {
        return 1.0 / (1 + pow(10, (opponentRating - engineRating) / 400));
    }

    int updateEloRating(int engineRating, int opponentRating, bool engineWon)
    {
        double expectedScore = calculateExpectedScore(engineRating, opponentRating);
        double actualScore = engineWon ? 1.0 : 0.0;
        double kFactor = 42; // Adjust the K-factor as needed

        // Update the Elo rating based on the outcome
        return static_cast<int>(engineRating + kFactor * (actualScore - expectedScore));
    }

    void Reset(){
        ShowMoveHistory = false;
        winner = NULL;
        SaveData = true;
    }

    void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color)
    {
        Vector2 Position = { posX, posY };
        DrawTextEx(myFont, text, Position, fontSize, 1.0, color);
    }

    Vector2 TextCenter(const char* text, float fontSize){
        return MeasureTextEx(myFont, text, fontSize, 1.0);
    }

    void DrawEvaluationColumn(ChessBoard& chessboard, ChessEngine& engine){
        // Calculate evaluation scores for both players
        float whiteScore = Evaluate(chessboard, White, engine);
        float blackScore = Evaluate(chessboard, Black, engine);

        // Calculate proportions of white and black color for the column
        float totalScore = whiteScore + blackScore;
        float whiteProportion = (totalScore != 0) ? (whiteScore / totalScore) : 0;
        float blackProportion = (totalScore != 0) ? (blackScore / totalScore) : 0;
        ostringstream White;
        ostringstream Black;
        White << fixed << setprecision(1) << whiteProportion;
        Black << fixed << setprecision(1) << blackProportion;
        string WhiteProp = White.str(); // White Proportions
        string BlackProp = Black.str(); // Black Proportions

        Rectangle columnRect = { 763, 80, 25, 640 }; // { x, y, width, height };

        // Draw the evaluation column with appropriate colors
        DrawRectangleRec(columnRect, WHITE); // Fill with white color
        Rectangle blackPart = { columnRect.x, columnRect.y, columnRect.width, columnRect.height * blackProportion };
        Rectangle whitePart = { columnRect.x, columnRect.y + blackPart.height, columnRect.width, columnRect.height * whiteProportion };
        DrawRectangleRec(whitePart, WHITE); // Fill with white color
        DrawRectangleRec(blackPart, BLACK); // Fill with black color
        DrawTextWithCustomFont(WhiteProp.c_str(), 765, 703, 15, BLACK);
        DrawTextWithCustomFont(BlackProp.c_str(), 765, 85, 15, WHITE);
    }

    float Evaluate(const ChessBoard& chessboard, int currentPlayerColor, ChessEngine& engine) const{
        // Piece values
        unordered_map<int, float> pieceValues = {
            {PAWN, 100}, {KNIGHT, 320}, {BISHOP, 330}, {ROOK, 500}, {QUEEN, 900}, {KING, 1000} };

        // Evaluate material advantage
        float selfMaterial = 0, opponentMaterial = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                if (piece.color == currentPlayerColor)
                {
                    selfMaterial += pieceValues[piece.type];
                }
                else
                {
                    opponentMaterial += pieceValues[piece.type];
                }
            }
        }
        float materialAdvantage = selfMaterial - opponentMaterial;

        // Evaluate positional advantage using PSTs
        float positionalAdvantage = 0;
        for (int i = 0; i < Total_tiles; ++i)
        {
            ChessPiece piece = chessboard.board[i];
            if (piece.type != EMPTY)
            {
                int pieceValue = engine.getPSTValue(piece, i, currentPlayerColor);
                positionalAdvantage += pieceValue;
            }
        }

        // Evaluate move options and capture moves
        vector<string> moves = chessboard.GetAllCaptureMovesInChessNotation(currentPlayerColor);
        int moveOptions = static_cast<int>(moves.size());
        int captureMoves = 0;
        for (const auto& move : moves)
        {
            if (engine.IsCaptureMove(move, chessboard))
            {
                captureMoves++;
            }
        }

        // Total evaluation is a combination of material, positional, move options, and capture moves
        float evaluation = abs(materialAdvantage + positionalAdvantage);
        evaluation += moveOptions * 10;  // Adjust weight as needed
        evaluation += captureMoves * 20; // Adjust weight as needed

        return evaluation;
    }
};



void CalculateAIMove(ChessEngine& engine, ChessBoard& board) {
    while (true) {
        if (engine.isSearchStarted() && !board.isCurrentPlayerWhite()) {

            string move;
            board.ComputeOpponentMoves();
            cout << "Running...\n";
            move = engine.GenerateMove(board);
            if (move != "") {
                engine.PlayMove(move, board);
            }
            engine.StopSearching();

        }

    }
}


class GameModes {

    ChessBoard chessboard;
    ChessEngine Horizon;
    BoardStats GameStats;
    Flags flags;
    User Player;
    bool DoOnce;
    string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";

public:
    GameModes() {
        DoOnce = true;
    }

    void InitialiseMultiplayerMode() {
        if (DoOnce) {
            PlaySound(GameStarts);
            chessboard.initializeBoardFromFEN(FENString);
            chessboard.ComputeOpponentMoves();
            DoOnce = false;
            flags.DisableSinglePlayer();
        }
    }

    void Options() {
        if (IsKeyPressed(KEY_H)) {
            GameStats.ShowMoveHistory = !GameStats.ShowMoveHistory;
        }
        if (IsKeyPressed(KEY_R)) RestartGame();
    }

    void HandleMoves(int PlayerColor) {
        for (int i = 0; i < Total_tiles; i++) {
            if (chessboard.board[i].color == PlayerColor) {
                chessboard.UpdateChessPiece(chessboard.board[i], i);
            }
        }
    }

    void MultiplayerMode() {
        InitialiseMultiplayerMode();
        if (!GameStats.GameIsEnded(chessboard)) { 
            if (chessboard.isCurrentPlayerWhite()) {
                HandleMoves(White);
            }
            else {
                HandleMoves(Black);
            }
            Options();
            if (GameStats.ShowMoveHistory) GameStats.DisplayMoveHistory(chessboard);
            else GameStats.DisplayStats(chessboard);
            DisplayBoard();

        }
        else {
            ResetBoard(false);
        }
    }

    void ResetBoard(bool calculateELO) {
        DisplayBoard();

        GameStats.DisplayEndMessage();
        Horizon.TerminateSearch();

        if (GameStats.SaveData) {
            if (calculateELO) CalculateELO();
            chessboard.saveMatchHistoryToFile();
            GameStats.SaveData = false;
        }
        if (IsKeyPressed(KEY_R)) RestartGame();
        if (IsKeyPressed(KEY_M)) BackToMenu();
    }

    void RestartGame() {
        Horizon.TerminateSearch();
        DoOnce = true;
        GameStats.Reset();
        Horizon.Reset();
        chessboard.DestroyBoard();
    }

    void BackToMenu() {
        DoOnce = false;
        GameStats.Reset();
        Horizon.Reset();
        chessboard.DestroyBoard();
        flags.EndGame();
    }

    void CalculateELO() {
            int oldBlackELO = Horizon.engineEloRating;
            Horizon.engineEloRating = GameStats.updateEloRating(Horizon.engineEloRating, Player.ELO, (GameStats.winner == Black));
            Player.ELO = GameStats.updateEloRating(Player.ELO, oldBlackELO, (GameStats.winner == White));
            Player.saveEloToFile();
            Horizon.saveEloToFile();
    }

    void InitialiseSinglePlayerMode() {
        if (DoOnce) {
            Horizon.setEngineColor(Black);
            thread aiThread(CalculateAIMove, ref(Horizon), ref(chessboard));
            aiThread.detach(); // Detach the thread to let it run independentl
            Player.setUserName("Taha");
            PlaySound(GameStarts);
            chessboard.initializeBoardFromFEN(FENString);
            chessboard.ComputeOpponentMoves();

            DoOnce = false;
            //isSinglePlayer = true;
        }

    }

    void SinglePlayerMode() {
        InitialiseSinglePlayerMode();
        if (!GameStats.GameIsEnded(chessboard)) {

            if (chessboard.isCurrentPlayerWhite()) HandleMoves(White);
            else Horizon.StartSearch();

            
            Options();
            if (GameStats.ShowMoveHistory) GameStats.DisplayMoveHistory(chessboard);
            else GameStats.DisplayStats(chessboard, Horizon, Player);
            DisplayBoard();
        }
        else ResetBoard(true);
    }
    
    void DisplayBoard() const {
        chessboard.DrawBoard();
        chessboard.DrawChessPiece();
    }

    void Destroy() {
        chessboard.DestroyBoard();
    }
};


//int main(){
//    InitWindow(screenWidth, screenHeight, "Chess");
//    InitAudioDevice();
//    ChessBoard chessboard;
//    BoardStats GameStats;
//    ChessEngine Gemini(Black);
//    GameModes Game;
//    
//    User Taha("Taha");
//
//    //string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
//    string FENString = "1rb3k1/p2q2bp/2p4r/2P1p1p1/3pPp2/1P1P1P2/PB1NN1KP/1R2QR2 b";
//    //string FENString = "qqqqkqqq/qqqqqqqq/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
//    // string FENString = "2k5/ppp5/8/8/8/8/2P1P1PP/4K3 w KQkq";
//    // string FENString = "8/p5k1/8/8/8/8/7P/7K w KQkq";
//    //  string FENString = "3rr3/3k4/8/8/3K4/8/8/8 w KQkq";
//    // string FENString = "4kbnr/8/8/8/8/8/8/5K1R w k";
//    // string FENString = "6Q1/8/8/8/8/8/K5R1/7k w KQkq";
//    // chessboard.initializeBoard();
//    //chessboard.initializeBoardFromFEN(FENString);
//    chessboard.initializeBoard();
//
//    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
//    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
//    KingChecked = LoadSound(KingCheckedFile.c_str());
//    GameStarts = LoadSound(GameStartsFile.c_str());
//    GameEnds = LoadSound(GameEndsFile.c_str());
//
//    Image icon = LoadImage(programIcon.c_str());
//
//    SetWindowIcon(icon);
//
//    myFont = LoadFont(fontFile.c_str());
//    string move;
//
//    bool DoneOnce = false;
//    SetTargetFPS(60);
//    bool gameStarted = false;
//    bool StartGame = false;
//
//    Rectangle InfoBox = { InfoBoxX, InfoBoxY, InfoBoxWidth, InfoBoxHeight };
//    Rectangle StartButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 3) - buttonCenterY, 200, 70 };
//    Rectangle MultiplayerButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 2) - buttonCenterY, 200, 70 };
//    Rectangle FENButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 1.5f) - buttonCenterY, 200, 70 };
//
//    thread aiThread(CalculateAIMove, ref(Gemini), ref(chessboard));
//    aiThread.detach(); // Detach the thread to let it run independentl
//
//    // Main menu loop
//    while (!WindowShouldClose()){
//        // Draw
//        BeginDrawing();
//        ClearBackground(Background);
//        DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent);
//        // Check if the game has started
//        if (gameStarted){
//
//            if (IsKeyPressed(KEY_SPACE)){
//                PlaySound(KingChecked);
//            }
//
//
//        else{
//     
//            if (StartGame){
//                gameStarted = true;
//            }
//            chessboard.DrawBoard();
//            chessboard.DrawChessPiece();
//
//            //DrawRectangleRec(StartButton, WHITE);
//            StartGame = GuiButton(StartButton, "Start");
//        }
//
//        EndDrawing();
//    }
//    chessboard.DestroyBoard();
//    UnloadSound(ChessPiecePlaced);
//    UnloadSound(ChessPieceCaptured);
//    UnloadSound(KingChecked);
//    UnloadSound(GameEnds);
//    UnloadSound(GameStarts);
//    UnloadFont(myFont);
//    UnloadImage(icon);
//    CloseAudioDevice();
//    CloseWindow();
//    return 0;
//}


int main() {
    InitWindow(screenWidth, screenHeight, "Chess");
    InitAudioDevice();

    Flags flags;

    GameModes Game;



    //string FENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    string FENString = "1rb3k1/p2q2bp/2p4r/2P1p1p1/3pPp2/1P1P1P2/PB1NN1KP/1R2QR2 b";
    //string FENString = "qqqqkqqq/qqqqqqqq/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    // string FENString = "2k5/ppp5/8/8/8/8/2P1P1PP/4K3 w KQkq";
    // string FENString = "8/p5k1/8/8/8/8/7P/7K w KQkq";
    //  string FENString = "3rr3/3k4/8/8/3K4/8/8/8 w KQkq";
    // string FENString = "4kbnr/8/8/8/8/8/8/5K1R w k";
    // string FENString = "6Q1/8/8/8/8/8/K5R1/7k w KQkq";
    // chessboard.initializeBoard();
    //chessboard.initializeBoardFromFEN(FENString);


    ChessPiecePlaced = LoadSound(ChessPiecePlacedFile.c_str());
    ChessPieceCaptured = LoadSound(ChessPieceCapturedFile.c_str());
    KingChecked = LoadSound(KingCheckedFile.c_str());
    GameStarts = LoadSound(GameStartsFile.c_str());
    GameEnds = LoadSound(GameEndsFile.c_str());

    Image icon = LoadImage(programIcon.c_str());

    SetWindowIcon(icon);

    myFont = LoadFont(fontFile.c_str());
    string move;

    //bool DoneOnce = false;
    SetTargetFPS(60);
    //bool gameStarted = false;
    bool StartGame = false;
    bool StartMultiplayer = false;

    Rectangle InfoBox = { InfoBoxX, InfoBoxY, InfoBoxWidth, InfoBoxHeight };
    Rectangle StartButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 3) - buttonCenterY, 200, 70 };
    Rectangle MultiplayerButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 2) - buttonCenterY, 200, 70 };
    Rectangle FENButton = { (InfoBoxWidth / 2) - 40, (InfoBoxHeight / 1.5f) - buttonCenterY, 200, 70 };


    // Main menu loop
    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
        ClearBackground(Background);
        DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent);
        // Check if the game has started
        if (flags.isGameStarted()) {

            if (flags.isSinglePlayer()) {
                Game.SinglePlayerMode();
            }
            else if (flags.isMultiplayerGame()) {
                Game.MultiplayerMode();

            }
        }

        else {

            if (StartGame || StartMultiplayer) {
                flags.StartGame();
                if (StartGame) {
                    flags.SinglePlayerMode();
                }
                else if (StartMultiplayer) {
                    flags.MultiplayerMode();
                }
            }

            Game.DisplayBoard();


            StartGame = GuiButton(StartButton, "Start");
            StartMultiplayer = GuiButton(MultiplayerButton, "Multiplayer");

        }
        EndDrawing();
    }
    Game.Destroy();
    UnloadSound(ChessPiecePlaced);
    UnloadSound(ChessPieceCaptured);
    UnloadSound(KingChecked);
    UnloadSound(GameEnds);
    UnloadSound(GameStarts);
    UnloadFont(myFont);
    UnloadImage(icon);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
