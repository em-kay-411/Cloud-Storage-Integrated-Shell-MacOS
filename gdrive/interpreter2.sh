#!/bin/bash

cd gdrive
node ./download.js $1 ./temp/$1
cd temp
$2 $1
cd ..
rm -r ./temp/*
cd ..