#!/bin/bash

set -e

pushd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null

virtualenv pyenv
./pyenv/bin/pip2.7 install snappy

mkdir -p build
cd build
qmake -makefile ../qtknots.pro
make

mkdir -p pdcodes graphs pdpictures pictures

popd > /dev/null

echo "Done"
