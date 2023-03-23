#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

#include "env.hpp"
namespace fs = std::filesystem;
using namespace std;

string getDestinationName(string destination){
    int i = destination.length() - 1;
    while(destination[i] != '/'){
        i--;
    }

    return destination.substr(i+1, destination.length() - (i+1));
}

void switchGDrive(){
    
    string gcmd;
    string directoryName = "root";
    chdir(getenv("CIS_PATH"));
    // char temp[2048];
    // getcwd(temp, 2048);
    // cout<<temp<<endl;
    while(gcmd != "exit"){
        cout<<directoryName<<" > ";
        getline(cin, gcmd);
        removeEndSpaces(gcmd);

        if(gcmd == "ls"){
            string command = "./gdrive/ls.sh "  + directoryName;            
            system(command.c_str());
        }

        else if(gcmd.substr(0, 3) == "ls "){
            string command = "./gdrive/ls.sh " + gcmd.substr(3, gcmd.length() - 3);
            system(command.c_str());
        }

        else if(gcmd.substr(0, 4) == "dwd "){
            int i = gcmd.length() - 1;
            while(gcmd[i] != ' '){
                i--;
            }
            string source = gcmd.substr(4, i-4);
            string destination = gcmd.substr(i+1, gcmd.length() - i);
            string destName = getDestinationName(source);
            string command = "./gdrive/dwd.sh " + source + " " + destination + "/" + destName;
            system(command.c_str());
        }

        else if(gcmd.substr(0, 4) == "upd "){
            int i = gcmd.length() - 1;
            while(gcmd[i] != ' '){
                i--;
            }

            string source = gcmd.substr(4, i-4);
            string destination = gcmd.substr(i+1, gcmd.length() - i);
            string command = "./gdrive/up.sh " + source + " " + destination;
            system(command.c_str());
        }
    }


}