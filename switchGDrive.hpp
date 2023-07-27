#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <thread>

#include "env.hpp"
namespace fs = std::filesystem;
using namespace std;

void gDriveHelp()
{
    cout << "               Cloud Integrated Shell - v1.0" << endl;
    cout << "                      Powered by EmKay" << endl;
    cout << "ls [<directoryPath>]           -      List files" << endl;
    cout << "dwd <source> <destination>     -      Download source file/folder to local destination" << endl;
    cout << "upd <source> <destination>     -      Upload source file/folder to Google Drive" << endl;
    cout << "cd <path>                      -      Change Current Directory to the path in Google Drive" << endl;
    cout << "rm <path>                      -      Delete the file/folder from Google Drive" << endl;
    cout << "mv <source> <destination>      -      Move source file/folder to a destination on Google Drive" << endl;
    cout << "cp <source> <destination>      -      Copy source file/folder to a destination on Google Drive" << endl;
    cout << "mkdir <path>                   -      Create a new directory on given path in Google Drive" << endl;
    cout << "vim <path>                     -      Opens a file in vim editor" << endl;
    cout << "vi <path>                      -      Opens a file in vi editor" << endl;
    cout << "whoami                         -      Get info about THIS ;-)" << endl;
    cout << "pwd                            -      Get current current working directory" << endl;
    cout << "help                           -      Help page" << endl;
    cout << "exit                           -      Exit from Google Drive and enter into the local system" << endl;
    cout << "cat                            -      To display text from a file" << endl;
    cout << "** Can run code files written in C, C++, Python, Java, NodeJS, Shell and Python Scripts **" << endl;
    cout << "**            The compiler should be installed on the local system **" << endl;
}

string getDestinationName(string destination)
{
    int i = destination.length() - 1;
    while (i >= 0 && destination[i] != '/')
    {
        i--;
    }

    if (i < 0)
    {
        return destination;
    }

    return destination.substr(i + 1, destination.length() - (i + 1));
}

string get_gpath(string gpathFile)
{
    fstream newfile;
    newfile.open(gpathFile, ios::in);
    string tp;
    if (newfile.is_open())
    {
        getline(newfile, tp);
    }
    newfile.close();
    return tp;
}

bool isRootPath(string path)
{
    if (path.substr(0, 5) == "root/")
    {
        return true;
    }
    return false;
}

void startAuthServer()
{
    string cisPath = getenv("CIS_PATH");
    string command = "pm2 start " + cisPath + "/gdrive/authLocalServer.js";
    system(command.c_str());
}

void stopAuthServer()
{
    string cisPath = getenv("CIS_PATH");
    string command = "pm2 stop " + cisPath + "/gdrive/authLocalServer.js";
    system(command.c_str());
}

void switchGDrive()
{
    string gpathFile = "./gdrive/gpath.txt";
    string gcmd;
    string directoryName = "root";
    chdir(getenv("CIS_PATH"));
    startAuthServer();
    string gpath = "";
    queue<string> history;

    // char temp[2048];
    // getcwd(temp, 2048);
    // cout<<temp<<endl;
    while (gcmd != "exit")
    {
        gpath = get_gpath(gpathFile);
        cout << directoryName << "/" << gpath << " > ";
        getline(cin, gcmd);
        removeEndSpaces(gcmd);
        if (history.size() == 200)
        {
            history.pop();
            history.push(gcmd);
        }
        else
        {
            history.push(gcmd);
        }

        if (gcmd == "ls")
        {
            string command;
            if (gpath == "")
            {
                command = "./gdrive/ls.sh root";
            }
            else
            {
                command = "./gdrive/ls.sh " + gpath;
            }

            system(command.c_str());
        }

        else if (gcmd.substr(0, 3) == "ls ")
        {
            string command;
            string path = gcmd.substr(3, gcmd.length() - 3);
            if (isRootPath(path))
            {
                command = "./gdrive/ls.sh " + path.substr(5, path.length() - 5);
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/ls.sh " + gcmd.substr(3, gcmd.length() - 3);
                }
                else
                {
                    command = "./gdrive/ls.sh " + gpath + "/" + gcmd.substr(3, gcmd.length() - 3);
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 4) == "dwd ")
        {
            thread([gcmd, gpath](){
                int i = gcmd.length() - 1;
                while (gcmd[i] != ' ')
                {
                    i--;
                }
                string source = gcmd.substr(4, i - 4);
                string destination = gcmd.substr(i + 1, (gcmd.length() - (i + 1)));
                string destName = getDestinationName(source);
                string command;
                if (isRootPath(source))
                {
                    command = "./gdrive/dwd.sh " + source.substr(5, source.length() - 5) + " " + destination + "/" + destName;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/dwd.sh " + source + " " + destination + "/" + destName;
                    }
                    else
                    {
                        command = "./gdrive/dwd.sh " + (gpath + "/" + source) + " " + (destination + "/" + destName);
                    }
                }

                system(command.c_str());
            }).detach();
        }

        else if (gcmd.substr(0, 4) == "upd ")
        {
            thread([gcmd, gpath](){
                int i = gcmd.length() - 1;
                while (gcmd[i] != ' ')
                {
                    i--;
                }

                string source = gcmd.substr(4, i - 4);
                string destination = gcmd.substr(i + 1, gcmd.length() - i);
                string command;
                if (isRootPath(destination))
                {
                    command = "./gdrive/up.sh " + source + " " + destination.substr(5, destination.length() - 5);
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/up.sh " + source + " " + destination;
                    }
                    else
                    {
                        command = "./gdrive/up.sh " + source + " " + gpath + "/" + destination;
                    }
                }

                system(command.c_str());
            }).detach();            
        }

        else if (gcmd.substr(0, 3) == "cd ")
        {
            string path = gcmd.substr(3, gcmd.length() - 3);
            string command;
            if (path == "..")
            {
                command = "./gdrive/cdp.sh";
                system(command.c_str());
                continue;
            }
            if (isRootPath(path))
            {
                command = "./gdrive/cd.sh " + path.substr(5, path.length() - 5);
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/cd.sh " + path;
                }
                else
                {
                    command = "./gdrive/cd.sh " + gpath + "/" + path;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 3) == "rm ")
        {
            thread([gcmd, gpath](){
                string path = gcmd.substr(3, gcmd.length() - 3);
                string command;
                if (isRootPath(path))
                {
                    command = "./gdrive/rm.sh " + path.substr(5, path.length() - 5);
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/rm.sh " + path;
                    }
                    else
                    {
                        command = "./gdrive/rm.sh " + gpath + "/" + path;
                    }
                }
                system(command.c_str());
            }).detach();            
        }

        // Only absolute paths required
        else if (gcmd.substr(0, 3) == "mv ")
        {
            thread([gcmd, gpath](){
                int i = gcmd.length() - 1;
                while (gcmd[i] != ' ')
                {
                    i--;
                }
                string source = gcmd.substr(3, i - 3);
                string destination = gcmd.substr(i + 1, (gcmd.length() - (i + 1)));
                string command;
                if (gpath == "")
                {
                    string argv2 = source;
                    string argv3 = destination;
                    command = "./gdrive/mv.sh " + source + " " + destination;
                }
                else
                {
                    string argv2 = gpath + "/" + source;
                    string argv3 = destination;
                    command = "./gdrive/mv.sh " + source + " " + destination;
                }
                system(command.c_str());
            }).detach();            
        }

        // Only absolute paths
        else if (gcmd.substr(0, 3) == "cp ")
        {
            thread([gcmd, gpath](){
                int i = gcmd.length() - 1;
                while (gcmd[i] != ' ')
                {
                    i--;
                }
                string source = gcmd.substr(3, i - 3);
                string destination = gcmd.substr(i + 1, (gcmd.length() - (i + 1)));
                string command;
                if (gpath == "")
                {
                    string argv2 = source;
                    string argv3 = destination;
                    command = "./gdrive/cp.sh " + source + " " + destination;
                }
                else
                {
                    string argv2 = gpath + "/" + source;
                    string argv3 = destination;
                    command = "./gdrive/cp.sh " + source + " " + destination;
                }
                system(command.c_str());
            }).detach();            
        }

        else if (gcmd.substr(0, 6) == "mkdir ")
        {
            string name = gcmd.substr(6, gcmd.length() - 6);
            string command;
            if (isPath(name))
            {
                int i = name.length() - 1;
                while (name[i] != '/')
                {
                    i--;
                }
                string folderName = name.substr(i + 1, (name.length() - (i + 1)));
                string folderPath = name.substr(0, i);
                if (isRootPath(folderPath))
                {
                    command = "./gdrive/mkdir.sh " + folderName + " " + folderPath.substr(5, folderPath.length() - 5);
                }
                else
                {
                    command = "./gdrive/mkdir.sh " + folderName + " " + gpath + "/" + folderPath;
                }
            }
            else
            {
                command = "./gdrive/mkdir.sh " + name + " " + gpath;
            }
            system(command.c_str());
        }

        else if (gcmd == "whoami")
        {
            cout << "Google Drive" << endl;
        }

        else if (gcmd == "pwd")
        {
            cout << "gdrive:root/" << gpath << endl;
        }

        else if (gcmd.substr(0, 4) == "vim ")
        {
            string str = gcmd.substr(4, gcmd.length() - 4);
            string command;
            if (isPath(str))
            {
                int i = str.length() - 1;
                while (str[i] != '/')
                {
                    i--;
                }

                string name = str.substr(i + 1, str.length() - (i + 1));
                string path = str.substr(0, i);
                if (isRootPath(str))
                {
                    command = "./gdrive/vim.sh " + path.substr(5, path.length() - 5) + " " + name;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/vim.sh " + path + " " + name;
                    }
                    else
                    {
                        command = "./gdrive/vim.sh " + gpath + "/" + path + " " + name;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/vim2.sh " + str;
                }
                else
                {
                    command = "./gdrive/vim.sh " + gpath + " " + str;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 3) == "vi ")
        {
            string str = gcmd.substr(3, gcmd.length() - 3);
            string command;
            if (isPath(str))
            {
                int i = str.length() - 1;
                while (str[i] != '/')
                {
                    i--;
                }

                string name = str.substr(i + 1, str.length() - (i + 1));
                string path = str.substr(0, i);
                if (isRootPath(str))
                {
                    command = "./gdrive/vi.sh " + path.substr(5, path.length() - 5) + " " + name;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/vi.sh " + path + " " + name;
                    }
                    else
                    {
                        command = "./gdrive/vi.sh " + gpath + "/" + path + " " + name;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/vi2.sh " + str;
                }
                else
                {
                    command = "./gdrive/vi.sh " + gpath + " " + str;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 4) == "cat ")
        {
            string str = gcmd.substr(4, gcmd.length() - 4);
            string command;
            if (isPath(str))
            {
                int i = str.length() - 1;
                while (str[i] != '/')
                {
                    i--;
                }

                string name = str.substr(i + 1, str.length() - (i + 1));
                string path = str.substr(0, i);
                if (isRootPath(str))
                {
                    command = "./gdrive/cat.sh " + path.substr(5, path.length() - 5) + " " + name;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/cat.sh " + path + " " + name;
                    }
                    else
                    {
                        command = "./gdrive/cat.sh " + gpath + "/" + path + " " + name;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/cat2.sh " + str;
                }
                else
                {
                    command = "./gdrive/cat.sh " + gpath + " " + str;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 4) == "g++ " || gcmd.substr(0, 4) == "gcc " || gcmd.substr(0, 6) == "javac ")
        {
            int j = gcmd.length() - 1;
            while (gcmd[j] != ' ')
            {
                j--;
            }
            string compiler = gcmd.substr(0, j);
            string str = gcmd.substr(j + 1, gcmd.length() - (j + 1));
            string command;
            if (isPath(str))
            {
                int i = str.length() - 1;
                while (str[i] != '/')
                {
                    i--;
                }

                string name = str.substr(i + 1, str.length() - (i + 1));
                string path = str.substr(0, i);
                if (isRootPath(str))
                {
                    command = "./gdrive/compile.sh " + path.substr(5, path.length() - 5) + " " + name + " " + compiler;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/compile.sh " + path + " " + name + " " + compiler;
                    }
                    else
                    {
                        command = "./gdrive/compile.sh " + gpath + "/" + path + " " + name + " " + compiler;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/compile2.sh " + str + " " + compiler;
                }
                else
                {
                    command = "./gdrive/compile.sh " + gpath + " " + str + " " + compiler;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(gcmd.length() - 4, 4) == ".out")
        {
            string command;
            int i = gcmd.length() - 1;
            while (gcmd[i] != '/')
            {
                i--;
            }
            string name = gcmd.substr(i + 1, gcmd.length() - (i + 1));
            string path = gcmd.substr(0, i);
            if (path == ".")
            {
                path = "";
            }

            if (isPath(path))
            {
                if (isRootPath(path))
                {
                    command = "./gdrive/run.sh " + path.substr(5, path.length() - 5) + " " + name;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/run.sh " + path + " " + name;
                    }
                    else
                    {
                        command = "./gdrive/run.sh " + gpath + "/" + path + " " + name;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/run2.sh " + name;
                }
                else
                {
                    command = "./gdrive/run.sh " + gpath + " " + name;
                }
            }
            system(command.c_str());
        }

        else if (gcmd.substr(0, 5) == "node " || gcmd.substr(0, 7) == "python " || gcmd.substr(0, 8) == "python3 " || gcmd.substr(0, 9) == "nodemon ")
        {
            int j = gcmd.length() - 1;
            while (gcmd[j] != ' ')
            {
                j--;
            }
            string interpreter = gcmd.substr(0, j);
            string str = gcmd.substr(j + 1, gcmd.length() - (j + 1));
            string command;
            if (isPath(str))
            {
                int i = str.length() - 1;
                while (str[i] != '/')
                {
                    i--;
                }

                string name = str.substr(i + 1, str.length() - (i + 1));
                string path = str.substr(0, i);
                if (isRootPath(str))
                {
                    command = "./gdrive/interpreter.sh " + path.substr(5, path.length() - 5) + " " + name + " " + interpreter;
                }
                else
                {
                    if (gpath == "")
                    {
                        command = "./gdrive/interpreter.sh " + path + " " + name + " " + interpreter;
                    }
                    else
                    {
                        command = "./gdrive/interpreter.sh " + gpath + "/" + path + " " + name + " " + interpreter;
                    }
                }
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/interpreter2.sh " + str + " " + interpreter;
                }
                else
                {
                    command = "./gdrive/interpreter.sh " + gpath + " " + str + " " + interpreter;
                }
            }
            system(command.c_str());
        }

        else if (gcmd == "history")
        {
            int n = history.size();
            for (int i = 0; i < history.size(); i++)
            {
                string cmd = history.front();
                history.pop();
                cout << cmd << endl;
                history.push(cmd);
            }
        }

        else if (gcmd == "help")
        {
            gDriveHelp();
        }

        else if (gcmd == "exit")
        {
            ofstream file("./gdrive/gid.txt", ios::out);
            file << "";
            file.close();

            ofstream file2("./gdrive/gpath.txt", ios::out);
            file2 << "";
            file2.close();

            chdir(PATH);
            stopAuthServer();
        }
    }
}

// For commands directly from local
void returnToLocal()
{
    ofstream file("./gdrive/gid.txt", ios::out);
    file << "";
    file.close();

    ofstream file2("./gdrive/gpath.txt", ios::out);
    file2 << "";
    file2.close();

    chdir(PATH);
    stopAuthServer();
}

void execFromLocalGDrive(string cmd)
{
    string gpathFile = "./gdrive/gpath.txt";
    string directoryName = "root";
    chdir(getenv("CIS_PATH"));
    startAuthServer();
    string gpath = "";
    queue<string> history;

    // char temp[2048];
    // getcwd(temp, 2048);
    // cout<<temp<<endl;

    if (cmd == "ls")
    {
        string command;
        if (gpath == "")
        {
            command = "./gdrive/ls.sh root";
        }
        else
        {
            command = "./gdrive/ls.sh " + gpath;
        }

        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 3) == "ls ")
    {
        string command;
        string path = cmd.substr(3, cmd.length() - 3);
        if (isRootPath(path))
        {
            command = "./gdrive/ls.sh " + path.substr(5, path.length() - 5);
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/ls.sh " + cmd.substr(3, cmd.length() - 3);
            }
            else
            {
                command = "./gdrive/ls.sh " + gpath + "/" + cmd.substr(3, cmd.length() - 3);
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 4) == "dwd ")
    {
        thread([cmd, gpath](){
            int i = cmd.length() - 1;
            while (cmd[i] != ' ')
            {
                i--;
            }
            string source = cmd.substr(4, i - 4);
            string destination = cmd.substr(i + 1, (cmd.length() - (i + 1)));
            string destName = getDestinationName(source);
            string command;
            if (isRootPath(source))
            {
                command = "./gdrive/dwd.sh " + source.substr(5, source.length() - 5) + " " + destination + "/" + destName;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/dwd.sh " + source + " " + destination + "/" + destName;
                }
                else
                {
                    command = "./gdrive/dwd.sh " + (gpath + "/" + source) + " " + (destination + "/" + destName);
                }
            }

            system(command.c_str());
            returnToLocal();
            return;
        }).detach();        
    }

    else if (cmd.substr(0, 4) == "upd ")
    {
        thread([cmd, gpath](){
            int i = cmd.length() - 1;
            while (cmd[i] != ' ')
            {
                i--;
            }

            string source = cmd.substr(4, i - 4);
            string destination = cmd.substr(i + 1, cmd.length() - i);
            string command;
            if (isRootPath(destination))
            {
                command = "./gdrive/up.sh " + source + " " + destination.substr(5, destination.length() - 5);
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/up.sh " + source + " " + destination;
                }
                else
                {
                    command = "./gdrive/up.sh " + source + " " + gpath + "/" + destination;
                }
            }

            system(command.c_str());
            returnToLocal();
            return;
        }).detach();        
    }

    else if (cmd.substr(0, 3) == "cd ")
    {
        string path = cmd.substr(3, cmd.length() - 3);
        string command;
        if (path == "..")
        {
            command = "./gdrive/cdp.sh";
            system(command.c_str());
        }
        if (isRootPath(path))
        {
            command = "./gdrive/cd.sh " + path.substr(5, path.length() - 5);
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/cd.sh " + path;
            }
            else
            {
                command = "./gdrive/cd.sh " + gpath + "/" + path;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 3) == "rm ")
    {
        thread([cmd, gpath](){
            string path = cmd.substr(3, cmd.length() - 3);
            string command;
            if (isRootPath(path))
            {
                command = "./gdrive/rm.sh " + path.substr(5, path.length() - 5);
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/rm.sh " + path;
                }
                else
                {
                    command = "./gdrive/rm.sh " + gpath + "/" + path;
                }
            }
            system(command.c_str());
            returnToLocal();
            return;
        }).detach();        
    }

    // Only absolute paths required
    else if (cmd.substr(0, 3) == "mv ")
    {
        thread([cmd, gpath](){
            int i = cmd.length() - 1;
            while (cmd[i] != ' ')
            {
                i--;
            }
            string source = cmd.substr(3, i - 3);
            string destination = cmd.substr(i + 1, (cmd.length() - (i + 1)));
            string command;
            if (gpath == "")
            {
                string argv2 = source;
                string argv3 = destination;
                command = "./gdrive/mv.sh " + source + " " + destination;
            }
            else
            {
                string argv2 = gpath + "/" + source;
                string argv3 = destination;
                command = "./gdrive/mv.sh " + source + " " + destination;
            }
            system(command.c_str());
            returnToLocal();
            return;
        }).detach();        
    }

    // Only absolute paths
    else if (cmd.substr(0, 3) == "cp ")
    {
        thread([cmd, gpath](){
            int i = cmd.length() - 1;
            while (cmd[i] != ' ')
            {
                i--;
            }
            string source = cmd.substr(3, i - 3);
            string destination = cmd.substr(i + 1, (cmd.length() - (i + 1)));
            string command;
            if (gpath == "")
            {
                string argv2 = source;
                string argv3 = destination;
                command = "./gdrive/cp.sh " + source + " " + destination;
            }
            else
            {
                string argv2 = gpath + "/" + source;
                string argv3 = destination;
                command = "./gdrive/cp.sh " + source + " " + destination;
            }
            system(command.c_str());
            returnToLocal();
            return;
        }).detach();        
    }

    else if (cmd.substr(0, 6) == "mkdir ")
    {
        string name = cmd.substr(6, cmd.length() - 6);
        string command;
        if (isPath(name))
        {
            int i = name.length() - 1;
            while (name[i] != '/')
            {
                i--;
            }
            string folderName = name.substr(i + 1, (name.length() - (i + 1)));
            string folderPath = name.substr(0, i);
            if (isRootPath(folderPath))
            {
                command = "./gdrive/mkdir.sh " + folderName + " " + folderPath.substr(5, folderPath.length() - 5);
            }
            else
            {
                command = "./gdrive/mkdir.sh " + folderName + " " + gpath + "/" + folderPath;
            }
        }
        else
        {
            command = "./gdrive/mkdir.sh " + name + " " + gpath;
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 4) == "vim ")
    {
        string str = cmd.substr(4, cmd.length() - 4);
        string command;
        if (isPath(str))
        {
            int i = str.length() - 1;
            while (str[i] != '/')
            {
                i--;
            }

            string name = str.substr(i + 1, str.length() - (i + 1));
            string path = str.substr(0, i);
            if (isRootPath(str))
            {
                command = "./gdrive/vim.sh " + path.substr(5, path.length() - 5) + " " + name;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/vim.sh " + path + " " + name;
                }
                else
                {
                    command = "./gdrive/vim.sh " + gpath + "/" + path + " " + name;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/vim2.sh " + str;
            }
            else
            {
                command = "./gdrive/vim.sh " + gpath + " " + str;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 3) == "vi ")
    {
        string str = cmd.substr(4, cmd.length() - 4);
        string command;
        if (isPath(str))
        {
            int i = str.length() - 1;
            while (str[i] != '/')
            {
                i--;
            }

            string name = str.substr(i + 1, str.length() - (i + 1));
            string path = str.substr(0, i);
            if (isRootPath(str))
            {
                command = "./gdrive/vi.sh " + path.substr(5, path.length() - 5) + " " + name;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/vi.sh " + path + " " + name;
                }
                else
                {
                    command = "./gdrive/vi.sh " + gpath + "/" + path + " " + name;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/vi2.sh " + str;
            }
            else
            {
                command = "./gdrive/vi.sh " + gpath + " " + str;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 4) == "cat ")
    {
        string str = cmd.substr(4, cmd.length() - 4);
        string command;
        if (isPath(str))
        {
            int i = str.length() - 1;
            while (str[i] != '/')
            {
                i--;
            }

            string name = str.substr(i + 1, str.length() - (i + 1));
            string path = str.substr(0, i);
            if (isRootPath(str))
            {
                command = "./gdrive/cat.sh " + path.substr(5, path.length() - 5) + " " + name;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/cat.sh " + path + " " + name;
                }
                else
                {
                    command = "./gdrive/cat.sh " + gpath + "/" + path + " " + name;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/cat2.sh " + str;
            }
            else
            {
                command = "./gdrive/cat.sh " + gpath + " " + str;
            }
        }
        system(command.c_str());
        return;
    }

    else if (cmd.substr(0, 4) == "g++ " || cmd.substr(0, 4) == "gcc " || cmd.substr(0, 6) == "javac ")
    {
        int j = cmd.length() - 1;
        while (cmd[j] != ' ')
        {
            j--;
        }
        string compiler = cmd.substr(0, j);
        string str = cmd.substr(j + 1, cmd.length() - (j + 1));
        string command;
        if (isPath(str))
        {
            int i = str.length() - 1;
            while (str[i] != '/')
            {
                i--;
            }

            string name = str.substr(i + 1, str.length() - (i + 1));
            string path = str.substr(0, i);
            if (isRootPath(str))
            {
                command = "./gdrive/compile.sh " + path.substr(5, path.length() - 5) + " " + name + " " + compiler;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/compile.sh " + path + " " + name + " " + compiler;
                }
                else
                {
                    command = "./gdrive/compile.sh " + gpath + "/" + path + " " + name + " " + compiler;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/compile2.sh " + str + " " + compiler;
            }
            else
            {
                command = "./gdrive/compile.sh " + gpath + " " + str + " " + compiler;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(cmd.length() - 4, 4) == ".out")
    {
        string command;
        int i = cmd.length() - 1;
        while (cmd[i] != '/')
        {
            i--;
        }
        string name = cmd.substr(i + 1, cmd.length() - (i + 1));
        string path = cmd.substr(0, i);
        if (path == ".")
        {
            path = "";
        }

        if (isPath(path))
        {
            if (isRootPath(path))
            {
                command = "./gdrive/run.sh " + path.substr(5, path.length() - 5) + " " + name;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/run.sh " + path + " " + name;
                }
                else
                {
                    command = "./gdrive/run.sh " + gpath + "/" + path + " " + name;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/run2.sh " + name;
            }
            else
            {
                command = "./gdrive/run.sh " + gpath + " " + name;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd.substr(0, 5) == "node " || cmd.substr(0, 7) == "python " || cmd.substr(0, 8) == "python3 " || cmd.substr(0, 9) == "nodemon ")
    {
        int j = cmd.length() - 1;
        while (cmd[j] != ' ')
        {
            j--;
        }
        string interpreter = cmd.substr(0, j);
        string str = cmd.substr(j + 1, cmd.length() - (j + 1));
        string command;
        if (isPath(str))
        {
            int i = str.length() - 1;
            while (str[i] != '/')
            {
                i--;
            }

            string name = str.substr(i + 1, str.length() - (i + 1));
            string path = str.substr(0, i);
            if (isRootPath(str))
            {
                command = "./gdrive/interpreter.sh " + path.substr(5, path.length() - 5) + " " + name + " " + interpreter;
            }
            else
            {
                if (gpath == "")
                {
                    command = "./gdrive/interpreter.sh " + path + " " + name + " " + interpreter;
                }
                else
                {
                    command = "./gdrive/interpreter.sh " + gpath + "/" + path + " " + name + " " + interpreter;
                }
            }
        }
        else
        {
            if (gpath == "")
            {
                command = "./gdrive/interpreter2.sh " + str + " " + interpreter;
            }
            else
            {
                command = "./gdrive/interpreter.sh " + gpath + " " + str + " " + interpreter;
            }
        }
        system(command.c_str());
        returnToLocal();
        return;
    }

    else if (cmd == "help")
    {
        gDriveHelp();
        returnToLocal();
        return;
    }
}