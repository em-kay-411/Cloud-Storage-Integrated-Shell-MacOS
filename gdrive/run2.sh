#!/bin/bash

cd gdrive
node ./download.js $1 ./temp/$1
cd temp
chmod 777 ./$1
./$1
cd ..
rm -r ./temp/$1
cd ..