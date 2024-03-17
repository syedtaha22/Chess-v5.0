#include "../../headers/Other/GlobalVariables.h"

Sound ChessPiecePlaced;
Sound ChessPieceCaptured;
Sound KingChecked;
Sound GameStarts;
Sound GameEnds;

Font myFont;

//Window
int screenWidth = 1500;
int screenHeight = 800;

//Board
int boardSize = 8;
int tileSize = 80;
int Total_tiles = 64;
int MaxHistorySize = 100;
int ReverseOffset = 7;

int BoardOffsetX = 800;
int BoardOffsetY = 80;


//Variables For InfoBox
int Segments = 20;
float Roundedness = 0.02f;
float InfoBoxX = 60;
float InfoBoxY = 80;
float InfoBoxWidth = 610;
float InfoBoxHeight = 640;

float fontSize = 50;

Color lightSquare = { 238, 238, 210, 255 };
Color darkSquare = { 118, 150, 86, 255 };
Color Transparent = { 0, 0, 0, 0 };
Color Background = { 49, 46, 43, 255 };
Color Translucent = { 0, 0, 0, 64 };

Color MoveHighlight = Fade({ 255, 255, 0, 200 }, 0.5);
Color MovesForPieceHighLight = Fade({ 255, 0, 0, 200 }, 0.5);
Color MoveHighlightBlue = Fade({ 0, 0, 255, 200 }, 0.5);
Color NextMoveHighlight = Fade({ 0, 255, 0, 200 }, 0.5);

//enum class PromotionPiece { QUEEN, ROOK, KNIGHT, BISHOP };
//Rectangle PawnPromotionDialogue;
