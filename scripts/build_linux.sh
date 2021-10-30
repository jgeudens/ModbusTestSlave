#!/bin/bash
set -ex

mkdir -p release
cd release

cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. 
ninja

cmake --install ..