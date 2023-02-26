FROM ubuntu:latest

RUN apt-get -y update && apt-get install -y

RUN apt-get -y install clang && \
    apt-get -y install gcc 

RUN apt-get install -y libsdl2-dev && \
    apt-get install -y libsdl2-ttf-dev

COPY . /usr/src/pong-c

WORKDIR /usr/src/pong-c

# RUN bash pack.sh

