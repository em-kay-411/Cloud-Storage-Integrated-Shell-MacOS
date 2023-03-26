#!/bin/bash

cd gdrive
node ./download.js $1 ./temp/$1
cd temp
vim $1
cd ..
node ./delete.js $1
node ./upload.js ./temp/$1 $1
rm -r ./temp/$1
cd ..