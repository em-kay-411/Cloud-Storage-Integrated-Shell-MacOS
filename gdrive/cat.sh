#!/bin/bash

cd gdrive
node ./download.js $1/$2 ./temp/$2
cd temp
cat $2
cd ..
rm -r ./temp/$2
cd ..