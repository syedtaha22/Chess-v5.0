#ifndef USER_H
#define USER_H

#include "../Engine/ChessEngine.h"

class User{
public:
    std::string Username;
    int ELO;
    const std::string filename = "UserELO.txt";

    User(std::string name = "", int elo = EMPTY);

    void setUserName(std::string name);
};



#endif // !USER_H



