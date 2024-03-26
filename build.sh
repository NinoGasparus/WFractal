#!/bin/bash

clear

exec
g++ -O3 -o wfr main.cpp  helperFunctions/helperFunctions.cpp keyEvents/keyEvents.cpp -lsfml-graphics -lsfml-window -lsfml-system -mavx2 -march=native
