#!/bin/bash

cd gdrive
node ./download.js $1/$2 ./temp/$2
cd temp
vim $2
cd ..
node ./delete.js $1/$2
node ./upload.js ./temp/$2 $1
rm -r ./temp/$2
cd ..