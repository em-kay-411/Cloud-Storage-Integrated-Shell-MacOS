#include <iostream>
#include <string>
#include <dirent.h>
#include <queue>
#include <vector>
using namespace std;


char *homeDir = getenv("HOME");
char *PATH = homeDir;
string displayPATH = "~";
string cmd;
queue<string> history;