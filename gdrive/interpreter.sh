#!/bin/bash

cd gdrive
node ./download.js $1/$2 ./temp/$2
cd temp
$3 $2
cd ..
rm -r ./temp/*
cd ..