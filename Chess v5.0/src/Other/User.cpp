#include "../../headers/Other/User.h"

User::User(string name, int elo) : Username(name), ELO(elo) {
    //if No ELO is provided, get ELO from settings;
    if (ELO == 0) ELO = manager.get(manager.userElo);
    //if ELO is still Zero, meaning no prior setting has been saved, then assigne default
    if (ELO == 0) ELO = 500; //Default
}


void User::setUserName(string name) {
    Username = name;
    ELO = manager.get(manager.userElo);
    if (ELO == 0) ELO = 500; //Default

}



