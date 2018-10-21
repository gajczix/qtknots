#!/bin/bash

pushd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null

if [ -f Makefile ]; then
  make clean
fi

rm -rf .qmake.stash qtknots qtknots.app Makefile
rm -rf pdcodes graphs pdpictures pictures log_output.txt
rm -rf pyenv

popd > /dev/null

echo "Done"
