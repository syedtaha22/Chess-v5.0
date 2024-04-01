#ifndef USER_H
#define USER_H

#include "../Engine/ChessEngine.h"


class User : public EloManager {
public:
    string Username;
    int ELO = 500;

    User(string name = "");

    //From EloManager.h
    int readEloFromFile() override;
    void saveEloToFile() const override;

    void setUserName(string name);
};



#endif // !USER_H



