#!/bin/bash

pushd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null

if [ -f qtknots ]; then
  ./build/qtknots
else
  ./build/qtknots.app/Contents/MacOS/qtknots
fi

popd > /dev/null

echo "Done"
