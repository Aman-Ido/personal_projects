#!/bin/bash
echo " - Compiling your program ...."
gcc image_viewer.c -o image_viewer -lSDL2main -lSDL2 -lSDL2_image
echo " - Running your program ..."
./image_viewer
