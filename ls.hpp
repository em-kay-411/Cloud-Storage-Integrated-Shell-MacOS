#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include "cd.hpp"

using namespace std;

// char *homeDir = getenv("HOME");
// char *PATH = homeDir;
// string displayPATH = "~";
// string cmd;

void listAllFiles(const char *temp)
{
    DIR *dir = opendir(temp);
    if (dir == nullptr)
    {
        cout << "Error: Unable to open directory" << endl;
        return;
    }

    vector<string> files;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        files.push_back(entry->d_name);
    }

    sort(files.begin(), files.end());

    for (int i = 0; i < files.size(); i++)
    {
        cout << files[i] << " ";
    }
    cout << endl;

    closedir(dir);
}

void listFiles(const char *temp)
{
    DIR *dir = opendir(temp);
    if (dir == nullptr)
    {
        cout << "Error: Unable to open directory" << endl;
        return;
    }

    vector<string> files;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_name[0] != '.')
        {
            files.push_back(entry->d_name);
        }
    }

    sort(files.begin(), files.end());

    for (int i = 0; i < files.size(); i++)
    {
        cout << files[i] << " ";
    }
    cout << endl;

    closedir(dir);
}

void lsCMD(string cmd)
{
    removeEndSpaces(cmd);
    if (cmd.length() == 2)
    {
        const char *temp = static_cast<const char *>(PATH);
        listFiles(temp);
    }
    else if (cmd[2] == ' ')
    {
        if (cmd.substr(0, 4) == "ls -")
        {
            if (cmd[4] == 'a')
            {
                string path = convertToString(PATH);
                string tempPath;
                if (cmd[6] == '/')
                {
                    tempPath = path + cmd.substr(6, cmd.length() - 6);
                }
                else
                {
                    string slash = "/";
                    tempPath = path + slash + cmd.substr(6, cmd.length() - 6);
                }

                cout << tempPath;

                const char *temp = tempPath.c_str();
                listAllFiles(temp);
                return;
            }
            else
            {
                cout << "No such command found. Only basic Linux commands can be used" << endl;
                return;
            }
        }
        else
        {
            string path = convertToString(PATH);
            string tempPath;
            if (cmd[3] == '/')
            {
                tempPath = path + cmd.substr(3, cmd.length() - 3);
            }
            else
            {
                string slash = "/";
                tempPath = path + slash + cmd.substr(3, cmd.length() - 3);
            }

            const char *temp = tempPath.c_str();
            listFiles(temp);
        }
    }
}