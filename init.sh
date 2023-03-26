#!/bin/bash

echo "** Installing Node Packages **"
npm install
echo "** Setting permissions **"
chmod 777 ./gdrive/*.sh
echo "** Compiling source code **"
g++ main.cpp -std=c++2a -o CiShell.out
echo "** Done compiling **"
echo "THE SHELL IS NOW READY TO USE"