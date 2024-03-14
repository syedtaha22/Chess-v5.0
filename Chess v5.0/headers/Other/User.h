#ifndef USER_H
#define USER_H

#include "../Engine/ChessEngine.h"

class User : public ReadWrite {
public:
    string Username;
    int ELO = 500;

    User(string name = "");

    void setUserName(string name);

    int readEloFromFile();

    void saveEloToFile() const;
};



#endif // !USER_H



