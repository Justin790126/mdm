#!/bin/bash

rm -rf build
mkdir build
cp style.css ./build/
cd build
cmake ../
make
cd ../
