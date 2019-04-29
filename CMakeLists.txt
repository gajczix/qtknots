cmake_minimum_required(VERSION 3.9)
project(qtknots)

set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/local/opt/qt5")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

find_package(Qt5OpenGL)

add_executable(qtknots
        src/crossings.cpp
        src/crossings.h
        functions.cpp
        functions.h
        src/GLpart.cpp
        src/GLpart.h
        src/graphs.cpp
        src/graphs.h
        src/Knot.cpp
        src/Knot.h
        src/log.cpp
        src/log.h
        src/main.cpp
        src/math_basics/fourvector.h
        src/math_basics/fourvector.cpp
        src/math_basics/matrix.h
        src/math_basics/matrix.cpp
        src/math_basics/function.h
        src/math_basics/function.cpp
        src/math_basics/map.h
        src/math_basics/map.cpp
        src/math_basics/parametr.h
        src/math_basics/parametr.cpp
        src/matbase.cpp
        src/matbase.h
        src/modyfikatory.cpp
        src/modyfikatory.h
        src/obraz.cpp
        src/obraz.h)

target_link_libraries(qtknots Qt5::Widgets)
qt5_use_modules(qtknots Widgets OpenGL)
