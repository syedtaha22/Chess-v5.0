#pragma once
class ReadWrite
{

    virtual int readEloFromFile() = 0;
    virtual void saveEloToFile() const = 0;
};