#ifndef EloManager_H
#define EloManager_H

class EloManager {

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};

#endif // !EloManager_H


