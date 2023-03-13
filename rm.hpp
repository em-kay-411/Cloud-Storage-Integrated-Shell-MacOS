#include <cstdio>
#include <iostream>
#include <filesystem>

#include "mkdir.hpp"
using namespace std;
namespace fs = std::filesystem;


void rmCMD(string cmd)
{
    if (cmd.substr(3, 2) == "-r" || cmd.substr(3, 2) == "-R")
    {
        string file_path = cmd.substr(6, cmd.length() - 6);
        fs::path dir_path(file_path);

        if (fs::exists(dir_path))
        {
            char res;
            cout << "This will remove all the files and subdirectories on the mentioned path\n";
            cout << "Are you sure of performing this operation (y/n) ? ";
            cin >> res;
            while (res != 'y' && res != 'n')
            {
                cout << "Input should be a single character (y/n) ";
                cin >> res;
            }

            if (res == 'y')
            {
                fs::remove_all(dir_path);                
            }
            else{
                cout << "Directory not deleted and is safe" << endl;
            }           
        
        }
        else{
            cout<< "Invalid path"<<endl;
        }    
        
    }
    else{
        string file_path = cmd.substr(3, cmd.length() - 3);
        
        if(remove(file_path.c_str()) != 0){
            cout<<"No such file found. "<<endl;
        }
    }
}
