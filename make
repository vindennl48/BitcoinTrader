#!bin/sh

clear;clear
echo "Removing old program..."
rm main

echo "Compiling new program..."
g++ -o main     \
                \
    main.c      \
                \
    -std=c++11  \
    -pthread    \
    -O0


echo "Running Program..."
echo " "
./main
echo " "
echo "Program Ended..."

