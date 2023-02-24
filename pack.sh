#!/bin/bash 

set -xe

CC=gcc-12
FILE_EXTENSION=c
CFLAGS="-Wall -Wextra -std=c11 -pedantic"
PKGS="sdl2"
EXECUTABLE="app"
SRC=$(find "./src" -type f -name "*.c")

if [ $(uname) == "Darwin" ]; then
    PKGS="${PKGS} sdl2_ttf"
fi
if [ $(uname) == "Linux" ]; then
    PKGS="${PKGS} sdl2-ttf"
fi

${CC} ${CFLAGS} $(pkg-config --cflags ${PKGS}) -o ${EXECUTABLE} ${SRC} $(pkg-config --libs ${PKGS})