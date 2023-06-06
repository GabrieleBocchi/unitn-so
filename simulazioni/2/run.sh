#!/bin/bash

if [ $# -ne 2 ]; then
  exit 3
fi

if [[ -e $1 ]]; then
    rm "$1"
fi

make

./program "$1" "$2"

cat "$1"
