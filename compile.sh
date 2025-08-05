#!/bin/bash

g++ -std=c++23 -Wall -O3 -s *.cpp */*.cpp -o pongpongpong -D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image -pthread

#g++ -std=c++2a -Wall -g *.cpp */*.cpp -o pongpongpong.bin -D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image -pthread