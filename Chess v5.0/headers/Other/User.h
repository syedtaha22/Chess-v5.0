#ifndef USER_H
#define USER_H


//#include "../Board/GraphicalBoard.h"
//#include "../Board/ChessBoard.h"
#include "../Engine/ChessEngine.h"



class User{
public:
    string Username;
    int ELO;
    const string filename = "UserELO.txt";

    User(string name = "", int elo = EMPTY);

    void setUserName(string name);
};



#endif // !USER_H



