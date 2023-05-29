# Nash Equilibria Solver (WIP)

## Description

## Building
### Fetching Dependencies
- Install CMake, if not already done so
- Install Eigen by running `chmod +x install_eigen.sh` and then `./install_eigen.sh`

### Compiling
- `cmake CMakeLists.txt`
- `make`

## Running
Simply run the executable named `NESOLVER`.

It can be run in a few different manners:
- Running `./NESOLVER` solves a bi-matrix game, and will prompt the user to enter the payoff matrices, entry by entry
- Running `./NESOLVER -[b, z, s]` solves a bi-matrix, zero-sum, or symmetric game, respectively, and will prompt the user to enter the payoff matrices, entry by entry
- Running `./NESOLVER -b ROW_PLAYER_PAYOFFS.csv` solves a bi-matrix game where both players have identical payoff matrices
- Running `./NESOLVER -z ROW_PLAYER_PAYOFFS.csv` solves a zero-sum game, where the column player's payoff matrix is the negation of the entered matrix
- Running `./NESOLVER -s ROW_PLAYER_PAYOFFS.csv` solves a symmetric game, where the column player's payoff matrix is the transpose of the entered matrix
- Running `./NESOLVER -b ROW_PLAYER_PAYOFFS.csv COL_PLAYER_PAYOFFS.csv` solves a bi-matrix game
- Running `./NESOLVER -[z, s] ROW_PLAYER_PAYOFFS.csv COL_PLAYER_PAYOFFS.csv` is equivalent to running `./NESOLVER -[z, s] ROW_PLAYER_PAYOFFS.csv`, respectively

## Dependencies
### Build
- [CMake](https://cmake.org/)

### Libraries
- [Eigen](https://github.com/libigl/eigen), a template library for linear algebra + matrices
- `bits/stdc++.h` from the standard C++ library. This isn't an issue on Linux, but as MacOS uses clang, which does not include `bits/stdc++.h`, you must used one of these work-arounds [here](https://stackoverflow.com/questions/28994148/how-can-i-include-bits-stdc-in-xcode)
