#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include "ls.hpp"
using namespace std;



void CMDmkdir(string cmd){
    string tempPath = convertToString(PATH);
    string name = cmd.substr(6, cmd.length() - 6);

    mkdir(name.c_str(), 0777);
}
