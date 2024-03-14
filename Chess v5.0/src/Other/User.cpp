#include "User.h"

User::User(string name) : Username(name) {
    ELO = readEloFromFile();
}

void User::setUserName(string name) {
    Username = name;

}

int User::readEloFromFile() {
    cout << "TEst" << endl;
    ifstream inputFile;
    string filename = Username + "ELO.txt";
    inputFile.open(filename);

    int elo = ELO; // Default value in case the file cannot be opened

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
    ofstream outputFile;
    string filename = Username + "ELO.txt";
    outputFile.open(filename);

    int defaultElo = 500;

    if (outputFile.is_open())
    {

        outputFile << ELO << endl;
        outputFile.close();
        cout << "Elo rating saved to " << filename << endl;
    }
    else
    {
        cerr << "Unable to open file " << filename << " for writing. Creating a new file with default Elo rating." << endl;
        outputFile.open(filename);
        if (outputFile.is_open())
        {
            outputFile << defaultElo << std::endl;
            outputFile.close();
            cout << "New file created with default Elo rating: " << defaultElo << endl;
        }
        else
        {
            cerr << "Unable to create file " << filename << " for writing." << endl;
        }
    }
}

