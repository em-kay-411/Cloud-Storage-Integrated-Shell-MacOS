#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "whoami.hpp"
using namespace std;
namespace fs = std::filesystem;

void copyFile(const fs::path& source, const fs::path& destination) {
    ifstream source_file(source, ios::binary);
    if (!source_file) {
        cout<<"Unable to find source file"<<endl;
        return;
    }
    
    ofstream dest_file(destination, ios::binary);
    if (!dest_file) {
        cout<<"Unable to open destination file"<<endl;;
        return;
    }

    dest_file << source_file.rdbuf();

    source_file.close();
    dest_file.close();
}

void copy_directory(const fs::path& source, const fs::path& destination) {
    if (!fs::exists(destination)) {
        if (!fs::create_directory(destination)) {
            cout<<"Unable to create directory at the destination";
            return;
        }
    }

    for (auto& entry : fs::directory_iterator(source)) {
        const auto destination_path = destination / entry.path().filename();
        if (fs::is_directory(entry)) {
            copy_directory(entry, destination_path);
        } else {
            copyFile(entry, destination_path);
        }
    }
}

bool isValid(string cmd){
    removeEndSpaces(cmd);
    int spaces = 0;
    for(int i=0; i<cmd.length(); i++){
        if(cmd[i] == ' '){
            spaces++;
        }
        
    }

    if(spaces != 2){
        return false;
    }

    return true;
}

void cpCMD(string cmd){
    if(!isValid(cmd)){
        cout<<cmd<<endl;
        cout<<"Wrong command enetered. 2 arguments are required"<<endl;
    }
    else{
        int space = 3;
        while(cmd[space] != ' '){
            space++;
        }    

        string sourceName = cmd.substr(3, space-3);
        string destinationName = cmd.substr(space+1, cmd.length() - space - 1);

        const fs::path source(sourceName);
        const fs::path destination(destinationName);

        if (fs::is_directory(source)) {
            copy_directory(source, destination);
        } else {
            if(fs::exists(source) && fs::is_directory(destination)){
                const auto new_path = destination / source.filename();
                copyFile(source, new_path);
            }
            else{
                copyFile(source, destination);
            }
        }


    }
}


