#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void preprocess(const string& inputFilePath) {
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        cerr << "Failed to create the temporary file." << endl;
        inputFile.close();
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        // Check if the line contains "#include" or "using namespace std;"
        if (line.find("#include") != string::npos || line.find("using namespace std;") != string::npos) {
            continue; // Skip lines containing #include or using namespace std;
        }
        tempFile << line << endl;
    }

    inputFile.close();
    tempFile.close();

    // Replace the input file with the temporary file
    remove(inputFilePath.c_str());
    rename("temp.txt", inputFilePath.c_str());
}

// Note: No closing brace here
