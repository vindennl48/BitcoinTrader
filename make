#!bin/sh

rm main
g++ -o main genetics.c brain.c main.c -std=c++11 -pthread
./main

