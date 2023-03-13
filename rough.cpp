#include <cstdio>
#include <iostream>
#include <string>
// #include <filesystem>

using namespace std;



int main() {
    string file_path="remove.pdf";
    remove(file_path.c_str());
    return 0;
}
