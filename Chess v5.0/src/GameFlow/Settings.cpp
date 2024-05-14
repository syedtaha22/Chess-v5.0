#include "../../headers/GameFlow/Settings.h"


void Settings::save(int depth, int userElo, int engineElo) {
    std::string appdataFolder = std::getenv("APPDATA");
    if (appdataFolder.empty()) {
        std::cerr << "Error: Unable to retrieve user's appdata folder path." << std::endl;
        return;
    }

    std::string folder = appdataFolder + "\\" + "ChessData";

    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directory(folder);
        //saveToFile(1, 500, 500); //Defaults
    }


    std::string filePath = folder + "\\" + filename;
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open file " << filePath << " for writing." << std::endl;
        return;
    }

    Json::Value root;
    root["depth"] = depth;
    root["userElo"] = userElo;
    root["engineElo"] = engineElo;

    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(root, &outputFile);
    outputFile.close();

    std::cout << "Data saved to " << filePath << std::endl;
}


int Settings::get(const std::string& element) {
    const int defaultVal = 0;
    std::string appdataFolder;
    const char* appdata = std::getenv("APPDATA");
    if (appdata != nullptr) {
        appdataFolder = appdata;
    }
    else {
        std::cerr << "Error: APPDATA folder not found(Using a different OS)" << std::endl;
    }

    if (appdataFolder.empty()) {
        std::cerr << "Error: Unable to retrieve user's appdata folder path." << std::endl;
        return defaultVal; // Return default rating
    }

    std::string folder = appdataFolder + "\\" + "ChessData";

    if (!std::filesystem::exists(folder)) {
        std::cerr << "Folder " << folder << " does not exist. Using default value." << std::endl;
        return defaultVal; // Return default rating
    }

    std::string filePath = folder + "\\" + filename;
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file " << filePath << " for reading. Using default value." << std::endl;
        return defaultVal;
    }

    Json::Value root;
    Json::CharReaderBuilder readerBuilder;
    std::string errors;
    if (!Json::parseFromStream(readerBuilder, inputFile, &root, &errors)) {
        std::cerr << "Error parsing JSON: " << errors << std::endl;
        inputFile.close();
        return defaultVal;
    }

    // Access the "depth" element and get its value
    int value = root[element].asInt();
    std::cout << element << " read from " << filePath << ": " << value << std::endl;
    return value;
}

void Settings::saveElement(const std::string& element, int value) {
    std::string appdataFolder = std::getenv("APPDATA");
    std::string folder = appdataFolder + "\\" + "ChessData";
    std::string filePath = folder + "\\" + filename;

    // Open the file for reading
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file " << filePath << " for reading." << std::endl;
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder readerBuilder;
    std::string errors;
    if (!Json::parseFromStream(readerBuilder, inputFile, &root, &errors)) {
        std::cerr << "Error parsing JSON: " << errors << std::endl;
        inputFile.close();
        return;
    }

    // Modify or add the specific element in the JSON object
    root[element] = value;

    // Write the modified JSON object back to file
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open file " << filePath << " for writing." << std::endl;
        inputFile.close(); // Close the input file stream
        return;
    }

    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(root, &outputFile);
    outputFile.close(); // Close the output file stream
    inputFile.close(); // Close the input file stream

    std::cout << "Element '" << element << "' saved with value " << value << " to " << filePath << std::endl;
}




const std::string Settings::filename = "settings.json";
std::string Settings::depth = "depth";
std::string Settings::engineElo = "engineElo";
std::string Settings::userElo = "userElo";