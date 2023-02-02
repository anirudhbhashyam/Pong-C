#!/bin/bash 

set -xe

CC=gcc-12
FILE_EXTENSION=c
CFLAGS="-Wall -Wextra -std=c11 -pedantic"
PKGS="sdl2 sdl2_ttf"
FILENAME=${1%.*}

${CC} ${CFLAGS} $(pkg-config --cflags ${PKGS}) -o ${FILENAME} ${1} $(pkg-config --libs ${PKGS})