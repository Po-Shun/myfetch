#!/bin/bash

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Navigate to the build directory
cd build

# Run CMake from the build directory
cmake ..

# Build the project using the default build tool
cmake --build .

cp myfetch ../