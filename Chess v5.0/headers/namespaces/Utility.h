#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "../Other/User.h"
#include "../Other/Resources.h"


namespace Utility {
	std::string AddCommas(int number);
	std::string CloseInBrackets(std::string stringClose);

	//Draw Text with Roboto font 
	void DrawTextWithCustomFont(const char* text, float posX, float posY, float fontSize, Color color);

	//sets floating point precisions
	std::string SetPrecision(const float& number, const int& precision);

	//returns the cordinates of center of a text. that can my used to position the text properly
	Vector2 TextCenter(const char* text, float fontSize);

	void UnloadFontsAndSounds();

	void LoadFontsAndSounds();

	//Supress Raylib Log messages
	void SuppressRaylibLog(int msgType, const char* text, va_list args);

	void Engine_Calculations(ChessEngine& engine, ChessBoard& board);

}

#endif // !HELPER_FUNCTIONS_H

