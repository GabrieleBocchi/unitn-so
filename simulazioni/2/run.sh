#!/bin/bash

if [[ -e $1 ]]; then
    rm "$1"
fi

make

./program "$1" "$2"

cat "$1"
