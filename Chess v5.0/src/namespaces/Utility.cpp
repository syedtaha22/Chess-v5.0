#include "../../headers/namespaces/Utility.h"



std::string Utility::AddCommas(int number) {
    std::ostringstream formatted;
    std::string str = std::to_string(number);
    size_t len = str.length();

    if (len <= 3) // No need for formatting if the number is less than 1000
        return str;

    size_t commas = (len - 1) / 3; // Calculate the number of commas to be inserted

    size_t firstSegmentLength = len % 3 == 0 ? 3 : len % 3; // Length of the first segment
    formatted << str.substr(0, firstSegmentLength);

    for (size_t i = firstSegmentLength; i < len; i += 3) {
        formatted << ',';
        formatted << str.substr(i, 3);
    }

    return formatted.str();
}

std::string Utility::CloseInBrackets(std::string stringClose) {
    std::string temp = "(";
    temp += stringClose;
    temp += ")";
    return temp;
}

std::string Utility::SetPrecision(const float& number, const int& precision) {
    std::ostringstream temp;
    temp << std::fixed << std::setprecision(precision) << number;
    return temp.str();

}

void Utility::DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color) {
    Vector2 Position = { posX, posY };
    DrawTextEx(myFont, text, Position, fontSize, 1.0, color);
}

Vector2 Utility::TextCenter(const char* text, float fontSize) {
    return MeasureTextEx(myFont, text, fontSize, 1.0);
}