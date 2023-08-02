#!/bin/bash

clang++ -Wall -Werror -std=c++14 _main.cpp space_ship.cpp bullet.cpp boundary.cpp asteroid.cpp particle.cpp engine_particle.cpp -Wno-deprecated-declarations -framework GLUT -framework OpenGL -framework Carbon -o _main\

./_main
