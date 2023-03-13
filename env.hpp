#include <iostream>
#include <cstdlib>
#include "mv.hpp"

using namespace std;
char **envp;
void printAllEnv() {
    
    char* env = *envp;

    
    while (env != nullptr) {
        cout << *env << endl;
        *env++;
    }
}