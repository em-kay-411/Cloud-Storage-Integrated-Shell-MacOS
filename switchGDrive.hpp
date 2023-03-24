#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

#include "env.hpp"
namespace fs = std::filesystem;
using namespace std;

string getDestinationName(string destination){
    int i = destination.length() - 1;
    while(i>=0 && destination[i] != '/'){
        i--;
    }

    if(i < 0){
        return destination;
    }

    return destination.substr(i+1, destination.length() - (i+1));
}

string get_gpath(string gpathFile){
    fstream newfile;
    newfile.open(gpathFile, ios::in);
    string tp;
    if(newfile.is_open()){
        getline(newfile, tp);
    }
    newfile.close();
    return tp;
}

void switchGDrive(){
    string gpathFile = "./gdrive/gpath.txt";
    string gcmd;
    string directoryName = "root";
    chdir(getenv("CIS_PATH"));
    string gpath = "";
     
    // char temp[2048];
    // getcwd(temp, 2048);
    // cout<<temp<<endl;
    while(gcmd != "exit"){
        gpath = get_gpath(gpathFile);
        cout<<directoryName<<"/"<<gpath<<" > ";
        getline(cin, gcmd);
        removeEndSpaces(gcmd);               

        if(gcmd == "ls"){
            string command;
            if(gpath == ""){
                command = "./gdrive/ls.sh root";
            }
            else{
                command = "./gdrive/ls.sh "  + gpath;
            }
                    
            system(command.c_str());
        }

        else if(gcmd.substr(0, 3) == "ls "){
            string command = "./gdrive/ls.sh " + gpath + "/" + gcmd.substr(3, gcmd.length() - 3);
            system(command.c_str());
        }

        else if(gcmd.substr(0, 4) == "dwd "){
            int i = gcmd.length() - 1;
            while(gcmd[i] != ' '){
                i--;
            }
            string source = gcmd.substr(4, i-4);
            string destination = gcmd.substr(i+1, (gcmd.length() - (i+1)));
            string destName = getDestinationName(source);
            string command;
            if(gpath == ""){
                command = "./gdrive/dwd.sh " + source + " " + destination + "/" + destName;
            }
            else{
                command = "./gdrive/dwd.sh " + (gpath + "/" + source) + " " + (destination + "/" + destName);
            }
            
            system(command.c_str());
        }

        else if(gcmd.substr(0, 4) == "upd "){
            int i = gcmd.length() - 1;
            while(gcmd[i] != ' '){
                i--;
            }

            string source = gcmd.substr(4, i-4);
            string destination = gcmd.substr(i+1, gcmd.length() - i);
            string command;
            if(gpath == ""){
                command = "./gdrive/up.sh " + source + " " + destination;
            }
            else{
                command = "./gdrive/up.sh " + source + " " + gpath + "/" + destination;
            } 
            system(command.c_str());
        }

        else if(gcmd.substr(0, 3) == "cd "){
            string path = gcmd.substr(3, gcmd.length() - 3);
            cout<<path<<endl;
            string command;
            if(path == ".."){
                command = "./gdrive/cdp.sh";
                system(command.c_str());
                continue;
            }
            if(gpath == ""){
                command = "./gdrive/cd.sh " + path;
            }
            else{
                command = "./gdrive/cd.sh " + gpath + "/" + path;
            }
            system(command.c_str());
        }

        else if(gcmd == "exit"){            
            ofstream file("./gdrive/gid.txt", ios::out);   
            file << "";   
            file.close();

            ofstream file2("./gdrive/gpath.txt", ios::out);
            file2<<"";
            file2.close();

            chdir(PATH);
        }
    }


}