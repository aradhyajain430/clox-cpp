#!/bin/bash
cmake -B build-asan -DCMAKE_BUILD_TYPE=Debug -DCLOX_SANITIZE=ON
cmake --build build-asan