#!/usr/bin/bash
g++ "${1}.cpp" -o "{$1}" -DBRUH -std=c++20 -Wall -fsanitize=undefined,address