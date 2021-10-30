#!/bin/bash
set -ex

export CXX="g++-9"
export CC="gcc-9"

mkdir -p release
cd release

cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. 
ninja
