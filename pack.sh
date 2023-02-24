#!/bin/bash 

set -xe

CC=gcc-12
FILE_EXTENSION=c
CFLAGS="-Wall -Wextra -std=c11 -pedantic"
PKGS="sdl2 sdl2_ttf"
EXECUTABLE="app"
SRC=$(find "./src" -type f -name "*.c")

if [ $(uname) == "Linux" ]; then
    PKGS="${PKGS} sdl2-ttf"
fi

${CC} ${CFLAGS} -o ${EXECUTABLE} ${SRC} $(pkg-config --cflags --libs ${PKGS})