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
  pathfinding algorithm guarantees the shortest path.

- **A\* Algorithm**: This pathfinding algorithm employs a heuristic to guide its way towards the goal. The key
  distinguishing factor between the A\\* and Dijkstra algorithms is that, whereas the Dijkstra's algorithm is eager and
  makes an attempt to get to the goal as quickly as possible, the A\\* algorithm also factors in the actual cost of the
  path alongside the heuristic, ensuring an efficient journey.

- **Jump Point Search**: This search algorithm is essentially an optimization of the A\\* algorithm for uniform-cost
  grid maps, and this is achieved without affecting optimality. Here, instead of examining each individual cell, the
  algorithm has the liberty to "jump" in between points in the direction of the goal, bypassing any irrelevant nodes.

## Obstacle Generation

For the generation of obstacles, the project currently supports four methods, which include:

- **Random**: In this method, the positioning of obstacles is randomized across the grid, which leads to no two grids
  being identical.

- **Wall-like**: This algorithm operates by generating either horizontal or vertical walls that span across the grid.
  The placement of these walls is randomized, and multiple walls could be created in the process.

- **Drunken Walk**: The Drunken Walk algorithm simulates a 'drunk' person walking across the grid, with each random
  direction resulting in an obstacle.

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
started all you need to do is clone the Git repository and then use the Devcontainer.

## Documentation

The documentation of this project has been done using Doxygen. To generate the documentation, install all the
dependencies and subsequently run the cmake pipeline. Please note that this readme only provides an overview of the
project. There is a vast array of parameters and settings that you can configure to customize your experience in regards
to pathfinding and obstacle generation. Detailed information about these aspects can be found in the code comments and
the Doxygen generated documentation.