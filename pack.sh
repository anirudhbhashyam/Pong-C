#!/bin/bash 

set -xe

CC=gcc
FILE_EXTENSION=c
# CFLAGS="-Wall -Wextra -std=c11 -pedantic -O3 -fprofile-instr-generate -fcoverage-mapping"
CFLAGS="-Wall -Wextra -std=c11 -pedantic -O3"
PKGS="sdl2"
EXECUTABLE="app"
SRC=$(find "./src" -type f -name "*.c")

if [ $(uname) == "Linux" ]; then
    PKGS="${PKGS} SDL2_ttf"
fi

if [ $(uname) == "Darwin" ]; then
    PKGS="${PKGS} SDL2_ttf"
fi 

${CC} ${CFLAGS} $(pkg-config --cflags ${PKGS}) -o ${EXECUTABLE} ${SRC} $(pkg-config --libs ${PKGS})