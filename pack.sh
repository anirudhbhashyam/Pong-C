set -xe

CC=gcc-12
FILE_EXTENSION=c
CFLAGS="-Wall -Wextra -std=c11 -pedantic"
FILENAME=${1%.*}

${CC} ${CFLAGS} -o ${FILENAME} ${1} $(pkg-config --cflags --libs sdl2 sdl2_ttf)