#!/bin/bash

cd gdrive
node ./download.js $1 ./temp/$1
cd temp
$2 $1
cd ..
node ./upload.js ./temp/a.out $1
rm -r ./temp/*
cd ..