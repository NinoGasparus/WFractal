#!/bin/bash

clear

exec
g++ -oo -o wfr main.cpp  helperFunctions/helperFunctions.cpp -lsfml-graphics -lsfml-window -lsfml-system
