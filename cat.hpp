#include <iostream>
#include <fstream>
#include <string>
#include "switchGDrive.hpp"
namespace fs = std::filesystem;
using namespace std;

void catCMD(string cmd){
    string filepath = cmd.substr(4, cmd.length() - 4);

    if (!fs::path(filepath).is_absolute()) {
        // Construct absolute path from relative path
        filepath = fs::absolute(filepath).string();
    }

    ifstream file(filepath);

    if (!file.is_open()) {
        cout << "Error opening file " << filepath << endl;
        return ;
    }

    string line;

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}