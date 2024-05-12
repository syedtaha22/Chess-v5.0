#include "../../headers/Other/GlobalVariables.h"

Sound ChessPiecePlaced;
Sound ChessPieceCaptured;
Sound KingChecked;
Sound GameStarts;
Sound GameEnds;

Font myFont;

Color lightSquare = { 238, 238, 210, 255 };
Color darkSquare = { 118, 150, 86, 255 };
Color Transparent = { 0, 0, 0, 0 };
Color Background = { 49, 46, 43, 255 };
Color Translucent = { 0, 0, 0, 64 };

Color MoveHighlight = Fade({ 255, 255, 0, 200 }, 0.5);
Color MovesForPieceHighLight = Fade({ 255, 0, 0, 200 }, 0.5);
Color MoveHighlightBlue = Fade({ 0, 0, 255, 200 }, 0.5);
Color NextMoveHighlight = Fade({ 0, 255, 0, 200 }, 0.5);

