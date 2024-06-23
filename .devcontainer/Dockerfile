FROM ubuntu:20.04

LABEL maintainer="Felix Renzikowski <st166260@stud.uni-stuttgart.de>"

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
    qt5-qmake


RUN apt-get upgrade -y

RUN git config --global core.filemode false
RUN git config --global core.autocrlf true

WORKDIR /workspace
