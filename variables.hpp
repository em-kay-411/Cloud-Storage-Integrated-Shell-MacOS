#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
using namespace std;

char *homeDir = getenv("HOME");
char *PATH = homeDir;
string displayPATH = "~";
string cmd;