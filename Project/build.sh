#!/bin/bash
RETRO68=/home/jao/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake
cmake -B ../build --toolchain ${RETRO68}
cd ../build
make
cd ../Project
