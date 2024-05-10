#ifndef STATISTICAL_CONSTANTS_H
#define STATISTICAL_CONSTANTS_H

#include "../namespaces/Utility.h"

struct StatisticalConstants {
    //Set a const Text Size for display
    const float TextSize = 50.0f;

    //Set a Reference point based on the Dimensions of The information Box on GUI

    const float textX = (InfoBoxWidth / 2.0f) + InfoBoxX;

    const float textY = 100.0f;

    //Calulate the Height and Width of Board
    const int BoardDimensions = tileSize * boardSize;

    //Colors
    const Color messageColor = WHITE;
    const Color AlertColor = RED;

    //Constant Messages, so that we don't have to right them each time
    const char* BlackInCheck = "Black's King is in Check";
    const char* WhiteInCheck = "White's King is in Check";

    // "ELO: "
    const char* ELO_text = "ELO: ";

    // "Black"
    const char* Black_ = "Black";

    // "White"
    const char* White_ = "White";

    // Kings Score
    const int MinScore = 10; 

    //Returns Position of "White" on the GUI
    std::pair<float, float> get_white_pos() {
        float w_posX = textX - (Utility::TextCenter(White_, fontSize).x / 2);
        float w_posY = textY - (Utility::TextCenter(White_, fontSize).y / 2) + 560;
        return std::make_pair(w_posX, w_posY);
    }

    //Returns Position of "Black" on the GUI
    std::pair<float, float> get_black_pos() {
        float b_posX = textX - (Utility::TextCenter(Black_, fontSize).x / 2);
        float b_posY = textY - (Utility::TextCenter(Black_, fontSize).y / 2) + 20;
        return std::make_pair(b_posX, b_posY);
    }

};

#endif // !STATISTICAL_CONSTANTS_H




