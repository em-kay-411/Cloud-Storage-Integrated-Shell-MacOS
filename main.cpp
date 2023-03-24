#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
#include "functions.hpp"

using namespace std;

int main()
{
    char CISpath[2048];
    getcwd(CISpath, 2048);
    setenv("CIS_PATH", CISpath, 1);

    while (cmd != "exit")
    {

        printPath(PATH);
        chdir(PATH);
        char temp[2048];
        getcwd(temp, 2048);
        PATH = temp;
        getline(cin, cmd);
        removeEndSpaces(cmd);
        if(history.size() == 200){
            history.pop();
            history.push(cmd);
        } 
        else{
            history.push(cmd);
        }

        if (cmd == "pwd")
        {
            char temp[2048];
            getcwd(temp, 2048);
            cout << temp << endl;
        }
        else if (cmd.substr(0, 2) == "ls")
        {
            lsCMD(cmd);
        }

        else if (cmd.substr(0, 3) == "cd ")
        {
            cdCMD(cmd);
        }

        else if (cmd == "exit")
        {
            cout << "Exiting CLI...." << endl;
            break;
        }

        else if (cmd.substr(0, 6) == "mkdir ")
        {
            CMDmkdir(cmd);
        }

        else if (cmd.substr(0, 3) == "rm ")
        {
            rmCMD(cmd);
        }

        else if (cmd.substr(0, 3) == "cp ")
        {
            cpCMD(cmd);
        }

        else if (cmd.substr(0, 3) == "mv ")
        {
            mvCMD(cmd);
        }

        else if (cmd == "env")
        {
            printAllEnv();
        }

        else if (cmd.substr(0, 9) == "printenv ")
        {
            removeEndSpaces(cmd);
            if (cmd == "printenv")
            {
                printAllEnv();
            }
            else
            {
                printEnv(cmd);
            }
        }

        else if (cmd == "whoami")
        {
            usernameCMD();
        }

        else if (cmd.substr(0, 7) == "switchd"){
            removeEndSpaces(cmd);
            string drive = cmd.substr(8, cmd.length() - 8);

            if(drive == "gdrive"){
                switchGDrive();
            }
        }

        else if (cmd.substr(0, 4) == "vim ")
        {
            system(cmd.c_str());
        }

        else if(cmd == "history"){
            int n = history.size();
            for(int i=0; i<history.size(); i++){
                string gcmd = history.front();
                history.pop();
                cout << gcmd << endl;
                history.push(gcmd);
            }
        }

        else if (cmd.substr(0, 3) == "vi ")
        {
            system(cmd.c_str());
        }

        else
        {
            cout << "No such command found. Only basic Linux commands can be used" << endl;
        }
    }

    return 0;
}