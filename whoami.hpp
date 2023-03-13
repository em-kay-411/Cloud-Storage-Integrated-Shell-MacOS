#include <iostream>
#include <unistd.h>
#include "rm.hpp"

void usernameCMD(){
    char * username = getlogin();

    cout<<username<<endl;
}