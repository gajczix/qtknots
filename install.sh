#!/bin/bash

set -e

pushd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null

mkdir -p build/no-graphics
cd ./build/no-graphics
cmake ../../src/no-graphics
make
cd ../../


virtualenv pyenv
./pyenv/bin/pip2.7 install snappy

qmake -makefile
make

mkdir -p pdcodes graphs pdpictures pictures

popd > /dev/null

echo "Done"
