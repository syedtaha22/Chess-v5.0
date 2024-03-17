#include "../../headers/Other/User.h"

User::User(string name) : Username(name) {
    if (Username != "") {
        ELO = readEloFromFile();
    }
}

void User::setUserName(string name) {
    Username = name;
    ELO = readEloFromFile();

}

int User::readEloFromFile() {
    string folder = "Data";
    if (!filesystem::exists(folder)) {
        cerr << "Folder " << folder << " does not exist. Using default Elo rating." << endl;
        return ELO;
    }

    string filename = folder + "/" + Username + "ELO.txt";
    ifstream inputFile(filename);

    int elo = ELO; 

    if (inputFile.is_open()) {
        inputFile >> elo;
        inputFile.close();
        cout << "Elo rating read from " << filename << ": " << elo << endl;
    }
    else {
        cerr << "Unable to open file " << filename << " for reading. Using default Elo rating." << endl;
    }

    return elo;
}

void User::saveEloToFile() const {
    string folder = "Data";
    
    if (!filesystem::is_directory(folder)) {
        filesystem::create_directory(folder);
    }

    ofstream outputFile;
    string filename = folder + "/" + Username + "ELO.txt";
    outputFile.open(filename);

    int defaultElo = 500;

    if (outputFile.is_open()) {
        outputFile << ELO << endl;
        outputFile.close();
        cout << "Elo rating saved to " << filename << endl;
    }
    else {
        cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
        outputFile.open(filename);
        if (outputFile.is_open()) {
            outputFile << defaultElo << endl;
            outputFile.close();
            cout << "New file created with default Elo rating: " << defaultElo << endl;
        }
        else {
            cerr << "Unable to create file " << filename << " for writing." << endl;
        }
    }
}

