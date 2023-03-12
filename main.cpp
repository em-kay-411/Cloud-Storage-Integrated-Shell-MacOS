#include<iostream>
#include<string>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
using namespace std;

string PATH = getenv("HOME");
string displayPATH = "~";
string cmd;


void printPath(string PATH){        
    if(PATH == getenv("HOME")){
        displayPATH = "~ ";
        cout<<displayPATH;
    }
    else{
        cout<<displayPATH;
    }
}

int main(){    
    while(cmd != "exit"){
        printPath(PATH);
        break;
        
    }    
    return 0;
}