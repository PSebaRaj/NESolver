#!/bin/sh

# Eigen
url_eigen="https://gitlab.com/libeigen/eigen.git"
dir="libs/Eigen"
git clone "$url_eigen" "$dir"

cmake CMakeLists.txt
