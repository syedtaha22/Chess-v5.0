#ifndef READWRITE_H
#define READWRITE_H


class ReadWrite {

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};



#endif // !READWRITE_H


