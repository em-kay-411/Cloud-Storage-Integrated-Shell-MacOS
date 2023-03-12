#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
using namespace std;

char *homeDir = getenv("HOME");
char *PATH = homeDir;
string displayPATH = "~";
string cmd;