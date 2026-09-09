#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 filename.c"
    exit 1
fi

gcc "$1" -o main -lraylib -lm -lX11 && ./main