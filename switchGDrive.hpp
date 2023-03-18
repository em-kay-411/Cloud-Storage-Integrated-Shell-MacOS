#include <iostream>
#include <string>
#include <unistd.h>
#include "env.hpp"
using namespace std;

void switchGDrive(){
    
    string gcmd;
    string directoryName = "root";
    while(gcmd != "exit"){
        cout<<directoryName<<" >";
        getline(cin, gcmd);
        removeEndSpaces(gcmd);

        if(gcmd == "ls"){
            string command = "./gdrive/ls.sh "  + directoryName;
            system(command.c_str());
        }       
    }


}