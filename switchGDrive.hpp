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
    while(destination[i] != '/'){
        i--;
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
        cout<<directoryName<<" > ";
        getline(cin, gcmd);
        removeEndSpaces(gcmd);
        gpath = get_gpath(gpathFile);

        if(gcmd == "ls"){
            string command;
            if(gpath == ""){
                command = "./gdrive/ls.sh "  + directoryName;   
            }
            else{
                command = "./gdrive/ls.sh "  + gpath + "/" + directoryName;
            }         
            system(command.c_str());
        }

        else if(gcmd.substr(0, 3) == "ls "){
            string command = "./gdrive/ls.sh " + gcmd.substr(3, gcmd.length() - 3);
            system(command.c_str());
        }

        else if(gcmd.substr(0, 4) == "dwd "){
            int i = gcmd.length() - 1;
            while(gcmd[i] != ' '){
                i--;
            }
            string source = gcmd.substr(4, i-4);
            string destination = gcmd.substr(i+1, gcmd.length() - i);
            string destName = getDestinationName(source);
            string command;
            if(gpath == ""){
                string command = "./gdrive/dwd.sh " + source + " " + destination + "/" + destName;
            }
            else{
                string command = "./gdrive/dwd.sh " + (gpath + "/" + source) + " " + (destination + "/" + destName);
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
            string command;
            if(gpath == ""){
                command = "./gdrive/cd.sh " + path;
            }
            else{
                command = "./gdrive/cd.sh " + gpath + "/" + path;
            }
            system(command.c_str());
        }
    }


}