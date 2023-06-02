#!/bin/bash

list=$(ls)
echo "$list" | tac | tr '\n' ' '
echo ""
