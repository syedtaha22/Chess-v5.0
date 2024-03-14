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
//#include<atomic>
//#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

using namespace std;
using namespace chrono;


 
extern Sound ChessPiecePlaced;
extern Sound ChessPieceCaptured;
extern Sound KingChecked;
extern Sound GameStarts;
extern Sound GameEnds;

//extern string ChessPiecePlacedFile;
//extern string ChessPieceCapturedFile;
//extern string KingCheckedFile;
//extern string GameStartsFile;
//extern string GameEndsFile;
//extern string fontFile;
//extern string programIcon;

extern Font myFont;

extern int screenWidth;
extern int screenHeight;
extern int boardSize;
extern int tileSize;
extern int MaxHistorySize;
extern int ReverseOffset;

extern float fontSize;

extern int BoardOffsetX;
extern int BoardOffsetY;
extern int Segments;

extern float Roundedness;
extern float buttonWidth;
extern float buttonHeight;
extern float buttonCenterX;
extern float buttonCenterY;
extern float InfoBoxX;
extern float InfoBoxY;
extern float InfoBoxWidth;
extern float InfoBoxHeight;

extern bool DoneOnceperMove;

extern Rectangle PawnPromotionDialogue;

extern Color lightSquare;
extern Color darkSquare;
extern Color Transparent;
extern Color Background;
extern Color Translucent;

extern Color MoveHighlight;
extern Color MovesForPieceHighLight;
extern Color PreMoveHighlight;
extern Color NextMoveHighlight;

extern int offsets;

extern int Total_tiles;

enum class PromotionPiece;



#endif // !GLOBALVARIABLES_H



