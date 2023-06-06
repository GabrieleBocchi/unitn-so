#!/bin/bash

if [ $# -ne 2 ]; then
  exit 3
fi

make

./app "$1" "$2"
