#ifndef SETTINGS_H
#define SETTINGS_H

#include<iostream>
#include<filesystem>
#include<fstream>
#include "json/json.h"

using namespace std;

class Settings {
    static const string filename;
    
public:
    static string depth;
    static string engineElo;
    static string userElo;

    static void save(int depth, int userElo, int engineElo);
    static int get(const std::string& element);
    static void saveElement(const std::string& element, int value);
};

#endif // !SETTINGS_H


