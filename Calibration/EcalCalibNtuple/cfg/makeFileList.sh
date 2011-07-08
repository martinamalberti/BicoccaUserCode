#!/bin/bash

if [ -z "$1" ]; then
    echo usage: $0 directory
    exit
fi

if [ -f list.txt ]; then
    echo file list.txt already exists
    exit
fi

ls -l $1 | grep root | awk '{print "'$1'/"$9}' > list.txt
