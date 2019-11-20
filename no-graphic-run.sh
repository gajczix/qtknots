#!/bin/bash

pushd "$(dirname "${BASH_SOURCE[0]}" )" > /dev/null

case "$1" in
    -function)
        echo "reading function from file"
        ./build/no-graphics/qtknots-no-graphics 1 $2
        echo "done"
    ;;    
    -points)
        echo "reading points from file"  
        ./build/no-graphics/qtknots-no-graphics 0 $2
        echo "done"
    ;;

    *)
        echo "ERROR: "
        echo " "
        echo "provide mode to run from 2 options"
        echo "  -function"
        echo "  -points"
        echo "followed by filename with source"
        echo "for more information visit https://github.com/gajczix/qtknots"
    ;;
esac
popd > /dev/null
