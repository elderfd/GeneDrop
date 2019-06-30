FROM ubuntu:latest

RUN apt-get update && apt-get install -y cmake build-essential qtbase5-dev libboost-dev

COPY . /usr/local/src
WORKDIR /usr/local/src

RUN mkdir build
WORKDIR /usr/local/src/build

RUN cmake ..
RUN cmake --build .
