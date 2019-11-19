cmake_minimum_required(VERSION 3.9)
project(qtknots_no_graphics)

set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_executable(qtknots-no-graphics
        ../math_basics/fourvector.h
        ../math_basics/fourvector.cpp
        ../math_basics/matrix.h
        ../math_basics/matrix.cpp
        ../math_basics/function.h
        ../math_basics/function.cpp
        ../math_basics/map.h
        ../math_basics/map.cpp
        ../math_basics/parametr.h
        ../math_basics/parametr.cpp
        ../math_basics/matbase.cpp
        ../math_basics/matbase.h
        ../math_basics/sample-func.cpp
        ../math_basics/sample-func.h
        ../crossings.cpp
        ../crossings.h
        ../graphs.cpp
        ../graphs.h
        ../Knot.cpp
        ../Knot.h
        ../log.cpp
        ../log.h
        no-graphics-main.cpp
)
