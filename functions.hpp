#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include "variables.hpp"

#include "ls.hpp"

using namespace std;

void printPath(char *PATH);
string convertToString(char temp[]);
void cdCMD(string cmd);
void listAllFiles(const char *temp);
void listFiles(const char *temp);
void lsCMD(string cmd);
