#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>


using namespace std;

// char *homeDir = getenv("HOME");
// char *PATH = homeDir;
// string displayPATH = "~";
// string cmd;
void printPath(char *PATH)
{
    char temp[2048];
    getcwd(temp, 2048); // get the current working directory

    if (strcmp(PATH, homeDir) == 0) // if PATH is equal to homeDir
    {
        displayPATH = homeDir;
    }
    else
    {
        displayPATH = temp;
    }

    PATH = temp; // update the value of PATH with the current working directory

    cout << displayPATH << " % ";
}

bool isPath(string str){
    for(int i=0; i<str.length(); i++){
        if(str[i] == '/'){
            return true;
        }
    }

    return false;
}

void removeEndSpaces(string cmd){
    int i = cmd.length() - 1;
    while(cmd[i] == ' '){
        i--;
    }
    string temp = cmd.substr(0, i+1);
    cmd = temp;
}

string convertToString(char temp[])
{
    string ans = "";
    for (int i = 0; temp[i] != '\0'; i++)
    {
        ans = ans + temp[i];
    }

    return ans;
}
