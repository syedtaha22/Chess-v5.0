#ifndef USER_H
#define USER_H

#include "../Engine/ChessEngine.h"


class User : public ReadWrite {
public:
    string Username;
    int ELO = 500;

    User(string name = "");

    //From ReadWrite.h
    int readEloFromFile() override;
    void saveEloToFile() const override;

    void setUserName(string name);
};



#endif // !USER_H



