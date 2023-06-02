#!/bin/bash

for (( i = $#; i >= 1; i-- ))
do
    echo "${!i}"
done
