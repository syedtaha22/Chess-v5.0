#include "../../headers/Exceptions/KingNotFound.h"



KingNotFound::KingNotFound(int position) : position(position) {
	message = "KingNotFound: " + std::to_string(position);
}

const char* KingNotFound::what() const noexcept {
	return message.c_str();
}
