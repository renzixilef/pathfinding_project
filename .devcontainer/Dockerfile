FROM ubuntu:22.04

LABEL maintainer="Felix Renzikowski <st166260@stud.uni-stuttgart.de>"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    cmake \
    doxygen \
    texlive-full \
    git \
    graphviz \
    qtcreator \
    qtbase5-dev \
    qt5-qmake \
    libgtest-dev \
    libopencv-dev

RUN mkdir /usr/src/gtest/build
RUN cd /usr/src/gtest/build && cmake .. && make && cd lib && cp *.a /usr/lib
RUN rm -r /usr/src/gtest/build

RUN apt-get upgrade -y

RUN git config --global core.filemode false
RUN git config --global core.autocrlf true

WORKDIR /workspace
