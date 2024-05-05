#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

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
#include <filesystem>
#include "raylib.h"


#include "Flags.h"
#include "Settings.h"


using namespace std;
using namespace chrono;


 
extern Sound ChessPiecePlaced;
extern Sound ChessPieceCaptured;
extern Sound KingChecked;
extern Sound GameStarts;
extern Sound GameEnds;

extern Font myFont;

//Window
extern int screenWidth;
extern int screenHeight;

//Board Variables
extern int boardSize;
extern int tileSize;
extern int Total_tiles;
extern int MaxHistorySize;
extern int ReverseOffset;
extern int BoardOffsetX;
extern int BoardOffsetY;

const int infinity = numeric_limits<int>::max();

//InfoBox
extern int Segments;
extern float Roundedness;
extern float InfoBoxX;
extern float InfoBoxY;
extern float InfoBoxWidth;
extern float InfoBoxHeight;

extern float fontSize;

extern Color lightSquare;
extern Color darkSquare;
extern Color Transparent;
extern Color Background;
extern Color Translucent;

extern Color MoveHighlight;
extern Color MovesForPieceHighLight;
extern Color MoveHighlightRed;
extern Color NextMoveHighlight;



//enum class PromotionPiece;
//extern Rectangle PawnPromotionDialogue;

#endif // !GLOBALVARIABLES_H



/*

GlobalVariables.h ---> Flags.h, Settings.h
ChessPiece.h ---> PieceCodes.h, GlobalVariables.h
Pieces.h ---> ChessPiece.h
ChessBoard.h ---> Pieces.h
GraphicalBoard.h ---> ChessBoard.h
TranspositionTables.h ---> GraphicalBoard.h
ChessEngine.h ---> TranspositionTables.h, PieceSquareTables.h
User.h ---> ChessEngine.h
BoardStats.h ---> User.h
GamesModes.h ---> BoardStats.h, Menu.h
Menu.h ---> GlobalVariables.h
Classes:
    Settings, Flags, ChessPiece, ChessBoard, GraphicalBoard, TranspositionTables, ChessEngine, User, BoardStats, GameModes, Menu, 


*/