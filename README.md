# Pong-C
A simple implementation of the classic arcade game, Pong, in C.

## CI
![build](https://www.github.com/anirudhbhashyam/Pong-C/actions/workflows/build.yml/badge.svg)

# Usage

## Getting Dependencies

### Unix
```bash
$ sudo apt-get install libsld2-dev
$ sudo apt-get install libsdl2-ttf-dev 
$ ./app
```

### MacOS
```bash
$ brew install SDL2
$ brew install SDL2_ttf
```

## Running the App

```bash
$ git clone git@github.com/anirudhbhashyam/Pong-C
$ bash pack.sh 
$ ./app
```

# Dependencies
- `SDL2`
- `SDL_TTF`

# TODO
- Paddle and ball collision is not perfect. It should be independent of position velocity.
  Try bv = 4 and pv = 4. Collisions do not happen.

