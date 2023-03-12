#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include "utils.hpp"
using namespace std;

void cdCMD(string cmd)
{
    if (cmd.length() > 3)
    {
        string str = cmd.substr(3, cmd.length() - 3);
        // cout<<"str"<<str<<endl;
        string path = convertToString(PATH);
        // cout<<"path"<<path<<endl;
        if (str[0] == '/')
        {
            path = path + str;
        }
        else if (str[0] == '~')
        {
            path = homeDir + str.substr(1, str.length() - 1);
        }
        else
        {
            string slash = "/";
            path = path + slash + str;
        }
        char *checkPath = const_cast<char *>(path.c_str());
        // cout<<"checkPath"<<checkPath<<endl;
        if (chdir(checkPath) != 0)
        {
            cout << "No such directory found" << endl;
            return;
        }
        PATH = checkPath;
        // cout<<"PATH ofter cd "<<PATH<<endl;
    }
}