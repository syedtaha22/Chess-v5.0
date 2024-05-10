#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "../Other/User.h"

namespace Utility {
	std::string AddCommas(int number);
	std::string CloseInBrackets(std::string stringClose);

	//Draw Text with Roboto font 
	void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color);

	std::string SetPrecision(const float& number, const int& precision);

	Vector2 TextCenter(const char* text, float fontSize);






}

#endif // !HELPER_FUNCTIONS_H

