#!/bin/bash

cd gdrive
node ./download.js $1/$2 ./temp/$2
cd temp
$3 $2
cd ..
node ./upload.js ./temp/a.out $1
rm -r ./temp/*
cd ..