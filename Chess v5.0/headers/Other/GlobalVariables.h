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
#include <memory>
#include <cstddef> // for size_t
#include "raylib.h"

#include "../GameFlow/Flags.h"
#include "../GameFlow/Settings.h"
#include "Timer.h"
 
extern Sound ChessPiecePlaced;
extern Sound ChessPieceCaptured;
extern Sound KingChecked;
extern Sound GameStarts;
extern Sound GameEnds;

extern Font myFont;

//Window
constexpr int screenWidth = 1500;
constexpr int screenHeight = 800;

//Board
constexpr int boardSize = 8;
constexpr int tileSize = 80;
constexpr int Total_tiles = 64;
constexpr int MaxHistorySize = 100;
constexpr int ReverseOffset = 7;

constexpr int BoardOffsetX = 800;
constexpr int BoardOffsetY = 80;

constexpr int infinity = std::numeric_limits<int>::max();

//Variables For InfoBox
constexpr int Segments = 20;
constexpr float Roundedness = 0.02f;
constexpr float InfoBoxX = 60.0f;
constexpr float InfoBoxY = 80.0f;
constexpr float InfoBoxWidth = 610.0f;
constexpr float InfoBoxHeight = 640.0f;

constexpr float fontSize = 50;

extern Color lightSquare;
extern Color darkSquare;
extern Color Transparent;
extern Color Background;
extern Color Translucent;

extern Color MoveHighlight;
extern Color MovesForPieceHighLight;
extern Color MoveHighlightRed;
extern Color NextMoveHighlight;

#endif // !GLOBALVARIABLES_H