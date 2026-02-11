![Build and Documentation CI/CD](https://github.com/renzixilef/pathfinding_project/actions/workflows/full.yml/badge.svg)

# Pathfinding

This project is developed as a part of the "Effizient programmieren" module at the University of Stuttgart in the
Masters course Aerospace Engineering. The main goal of this project is to leverage and demonstrate various pathfinding
algorithms on 2D grids with certain obstacles. The project has been developed as a Qt5 application and can operate both
in GUI and headless modes.

## Implemented Algorithms

There are three key pathfinding algorithms that are currently implemented in this project. They are:

- **Dijkstra**: This is a renowned pathfinding algorithm that operates by exploring the neighbouring nodes of the start
  point. It then traverses towards the goal by frequently jumping to the node with the lowest cost. The Dijkstra
  pathfinding algorithm guarantees the shortest path. This algorithm is implemented in C++ and in Ada.

- **A\* Algorithm**: This pathfinding algorithm employs a heuristic to guide its way towards the goal. The key
  distinguishing factor between the A\* and Dijkstra's algorithm is that, while Dijkstra's algorithm seeks the shortest
  path to all possible destinations, the A\* algorithm uses a heuristic to estimate the cost to the goal, which allows
  it to focus on more promising paths and reach the goal more efficiently. In other words, A\* is more strategic in
  terms of selecting the next node for expansion, quickly finding an optimal path in large graphs compared to Dijkstra's
  algorithm. This algorithm is implemented in C++ and in Ada.

- **Jump Point Search**: This search algorithm is essentially an optimization of the A\* algorithm for uniform-cost
  grid maps, and this is achieved without affecting optimality. Here, instead of examining each individual cell, the
  algorithm has the liberty to "jump" in between points in the direction of the goal, bypassing any irrelevant nodes. 
  This algorithm is only implemented in C++.

## Obstacle Generation

For the generation of obstacles, the project currently supports four methods, which include:

- **Random**: In this method, the positioning of obstacles is randomized across the grid, which leads to no two grids
  being identical.

- **Wall-like**: This algorithm operates by generating either horizontal or vertical walls that span across the grid.
  The placement of these walls is randomized, and multiple walls could be created in the process.

- **Drunken Walk**: The Drunken Walk algorithm simulates a 'drunk' person wandering across the grid, with each random
  direction resulting in a walkable cell.

- **Perlin Noise**: This method generates natural-looking patterns of obstacles based on Perlin noise. It's particularly
  effective for creating random yet organic and smooth distributions of obstacles.

## Installation

### Dependencies

For the building process, the project requires the following dependencies:

```console
build-essential
gdb
cmake
qtcreator
qtbase5-dev
qt5-qmake
gprbuild
```

For the Doxygen documentation generation, you will need to have the following dependencies:

```console
doxygen
graphviz
graphicx
multicol
babel
amssymb
fancyhdr
inputenc
fontenc
```

### Devcontainer

In order to simplify the setup and usage process, a Devcontainer has been provided. This essentially allows you to use
the project inside a Docker environment. This isolates your development environment from your local machine. To get
started all you need to do is clone the Git repository and then use the Devcontainer. Building the Container may take some
time as a texlive-full installation is a part of the container to be able to build the documentation and PDF-Paper.
To build the project using the Devcontainer, set it up and then run the following commands in the devcontainer shell:
```console
cmake -GNinja -S . -B ./build -DWITH_DOCS=ON -DWITH_PAPER=ON
cmake --build ./build
```
To only build the executable and not the documentation and the PDF-Paper run the following commands instead:
```console
cmake -GNinja -S . -B ./build
cmake --build ./build
```
When operating on Windows, WSL can be used to execute the compiled binary afterwards.

## Documentation

The documentation of this project has been done using Doxygen. To generate the documentation, install all the
dependencies and subsequently run the cmake pipeline. Please note that this readme only provides an overview of the
project. There is a vast array of parameters and settings that you can configure to customize your experience in regards
to pathfinding and obstacle generation. Detailed information about these aspects can be found in the code comments and
the Doxygen generated documentation.