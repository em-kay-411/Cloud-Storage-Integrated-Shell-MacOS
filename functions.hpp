#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include "variables.hpp"
#include "switchGDrive.hpp"
using namespace std;

void removeEndSpaces(string cmd);
bool isPath(string str);
void printPath(char *PATH);
string convertToString(char temp[]);
void cdCMD(string cmd);
void listAllFiles(const char *temp);
void listFiles(const char *temp);
void lsCMD(string cmd);
void CMDmkdir(string cmd);
void rmCMD(string cmd);
void usernameCMD();
void cpCMD(string cmd);
void mvCMD(string cmd);
void printAllEnv();
void switchGDrive();
