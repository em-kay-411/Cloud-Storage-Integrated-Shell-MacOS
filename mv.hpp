#include <iostream>
#include <filesystem>
#include "cp.hpp"
using namespace std;
namespace fs = std::filesystem;

void move_file(const fs::path& source, const fs::path& destination) {
  fs::rename(source, destination);
}

void move_directory(const fs::path& source, const fs::path& destination) {
  fs::rename(source, destination);
}



void mvCMD(string cmd){
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
            move_directory(source, destination);
        } else {
            if(fs::exists(source) && fs::is_directory(destination)){
                const auto new_path = destination / source.filename();
                move_file(source, new_path);
            }
            else{
                move_file(source, destination);
            }
        }


    }
}