#!/bin/bash

set -e

input=$1

out=0

while IFS= read -r line
do
    echo "$line" 1>&$((out % 2 + 1))
    out=$((out + 1))
done < "$input"
