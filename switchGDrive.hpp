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
            char *command = "node";
            char *argv[] = {command, "./gdrive/listfiles.js", const_cast<char*>(directoryName.c_str()), NULL};
            execv(command, argv);
        }       
    }


}