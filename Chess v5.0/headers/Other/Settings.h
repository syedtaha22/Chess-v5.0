#ifndef SETTINGS_H
#define SETTINGS_H

#include<iostream>
#include<filesystem>
#include<fstream>
#include "json/json.h"

class Settings {
    static const std::string filename;
    
public:
    static std::string depth;
    static std::string engineElo;
    static std::string userElo;

    static void save(int depth, int userElo, int engineElo);
    static int get(const std::string& element);
    static void saveElement(const std::string& element, int value);
};

#endif // !SETTINGS_H


