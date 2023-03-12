#!/bin/bash

if [ -z "$1" ]; then
    echo "No argument provided"
    exit 1
fi

vim "$1"