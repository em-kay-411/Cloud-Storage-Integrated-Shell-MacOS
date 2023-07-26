#include <iostream>
#include <unistd.h>
#include "variables.hpp"
#include "rm.hpp"

void usernameCMD(){
    removeEndSpaces(cmd);
    char * username = getlogin();

    cout<<username<<endl;
}