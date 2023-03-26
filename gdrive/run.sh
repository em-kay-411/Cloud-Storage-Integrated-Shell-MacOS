#!/bin/bash

cd gdrive
node ./download.js $1/$2 ./temp/$2
cd temp
chmod 777 ./$2
./$2
cd ..
rm -r ./temp/$2
cd ..