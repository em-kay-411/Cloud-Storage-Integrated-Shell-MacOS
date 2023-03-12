#include<iostream>
#include<string>
#include <cstdlib>
#include <unistd.h>
#include <sys/utsname.h>
using namespace std;

int main(){
    chdir("/Users/emkay/Desktop/COEP");
    char s[512];
    getcwd(s, 512);
    cout<<s;
    return 0;
}