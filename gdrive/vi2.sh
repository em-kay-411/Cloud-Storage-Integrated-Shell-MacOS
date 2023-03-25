#!/bin/bash

cd gdrive
node ./download.js $1 ./temp
cd temp
vi $1
cd ..
node ./upload.js ./temp/$1 $1
rm -r ./temp/$1
cd ..