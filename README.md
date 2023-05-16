# Nash Equilibria Solver

## Description

## Building
### Fetching Dependencies
- Install CMake, if not already done so
- Install Eigen by running `chmod +x install_eigen.sh` and then `./install_eigen.sh`
- `cmake .`
- `make`

## Running
Simply run the executable named `NESOLVER`.

It can be run in two different manners:
- With the row player and column player's matrices as CSVs, run the executable as: `./NESOLVER ROW_PLAYER_PAYOFFS.csv COL_PLAYER_PAYOFFS.csv`.
- If you would prefer to enter the values of the matrices one at a time, run the executable simply as `./NESOLVER`.

## Dependencies
### Build
- [CMake](https://cmake.org/)
- `bits/stdc++.h`. This isn't an issue on Linux, but as MacOS uses clang, which does not include `bits/stdc++.h`, you must used one of these work-arounds [here](https://stackoverflow.com/questions/28994148/how-can-i-include-bits-stdc-in-xcode)

### Libraries
- [Eigen](https://github.com/libigl/eigen), a template library for linear algebra + matrices
