#!/bin/bash

if command -v pm2 >/dev/null 2>&1 ; then
  echo "pm2 is already installed"
else
  echo "pm2 is not installed, installing now..."
  npm install pm2 -g && pm2 update
fi

echo "** Installing Node Packages **"
npm install
echo "** Setting permissions **"
chmod 777 ./gdrive/*.sh
echo "** Compiling source code **"
g++ main.cpp -std=c++2a -o CiShell.out
echo "** Done compiling **"
echo "THE SHELL IS NOW READY TO USE"