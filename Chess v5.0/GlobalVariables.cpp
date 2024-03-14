#include "GlobalVariables.h"

Sound ChessPiecePlaced;
Sound ChessPieceCaptured;
Sound KingChecked;
Sound GameStarts;
Sound GameEnds;



Font myFont;

int screenWidth = 1500;
int screenHeight = 800;
int boardSize = 8;
int tileSize = 80;
int MaxHistorySize = 100;
int ReverseOffset = 7;

float fontSize = 50;

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

Color MoveHighlight = Fade({ 255, 255, 0, 200 }, 0.5);
Color MovesForPieceHighLight = Fade({ 255, 0, 0, 200 }, 0.5);
Color PreMoveHighlight = Fade({ 0, 0, 255, 200 }, 0.5);
Color NextMoveHighlight = Fade({ 0, 255, 0, 200 }, 0.5);

int offsets;


int Total_tiles = 64;

enum class PromotionPiece { QUEEN, ROOK, KNIGHT, BISHOP };
