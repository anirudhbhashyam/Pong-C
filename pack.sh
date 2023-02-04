#!/bin/bash 

set -xe

CC=gcc-12
FILE_EXTENSION=c
CFLAGS="-Wall -Wextra -std=c11 -pedantic"
PKGS="sdl2 sdl2_ttf"
EXECUTABLE="app"
SRC=$(find "./src" -type f -name "*.c")

${CC} ${CFLAGS} $(pkg-config --cflags ${PKGS}) -o ${EXECUTABLE} ${SRC} $(pkg-config --libs ${PKGS})