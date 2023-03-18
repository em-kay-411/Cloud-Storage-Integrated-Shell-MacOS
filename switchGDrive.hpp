#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

#include "env.hpp"
namespace fs = std::filesystem;
using namespace std;

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
    }


}