#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "mv.hpp"

using namespace std;

void printAllEnv()
{
    extern char **environ;
    char **env = environ;

    while (*env != nullptr)
    {
        string varname(*env);
        string varvalue = getenv(varname.c_str());
        cout << varname << "=" << varvalue << endl;
        env++;
    }
}

void printEnv(string cmd)
{
    string varname = cmd.substr(9, cmd.length() - 9);
    const char *varvalue = getenv(varname.c_str());
    if (varvalue == nullptr)
    {
        cout << "Error: No such variable found" << endl;
    }
    else
    {
        cout << varname << " = " << varvalue << endl;
    }
}