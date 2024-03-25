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


using namespace std;
using namespace chrono;
 


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



//InfoBox
extern int Segments;
extern float Roundedness;
extern float InfoBoxX;
extern float InfoBoxY;
extern float InfoBoxWidth;
extern float InfoBoxHeight;

extern float fontSize;



//enum class PromotionPiece;
//extern Rectangle PawnPromotionDialogue;

#endif // !GLOBALVARIABLES_H



