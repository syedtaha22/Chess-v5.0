#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <json/json.h>

class Settings {
public:
    std::string userName;
    int userElo;
    int engineElo;
    int engineDepth;

    // Constructor
    Settings(const std::string& userName, int userElo, int engineElo, int engineDepth)
        : userName(userName), userElo(userElo), engineElo(engineElo), engineDepth(engineDepth) {}

    // Save settings to JSON file
    void saveToJson(const std::string& filename) const {
        Json::Value root;
        root["userName"] = userName;
        root["userElo"] = userElo;
        root["engineElo"] = engineElo;
        root["engineDepth"] = engineDepth;

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return;
        }

        file << root;
        file.close();
    }

    // Load settings from JSON file
    static Settings loadFromJson(const std::string& filename) {
        Settings settings("", 0, 0, 0);
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for reading." << std::endl;
            return settings;
        }

        Json::Value root;
        file >> root;
        file.close();

        settings.userName = root.get("userName", "").asString();
        settings.userElo = root.get("userElo", 0).asInt();
        settings.engineElo = root.get("engineElo", 0).asInt();
        settings.engineDepth = root.get("engineDepth", 0).asInt();

        return settings;
    }
};




#endif // !SETTINGS_H


