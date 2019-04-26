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
        crossings.cpp
        crossings.h
        functions.cpp
        functions.h
        GLpart.cpp
        GLpart.h
        graphs.cpp
        graphs.h
        Knot.cpp
        Knot.h
        log.cpp
        log.h
        main.cpp
        math_basics/fourvector.h
        math_basics/fourvector.cpp
        math_basics/matrix.h
        math_basics/matrix.cpp
        math_basics/function.h
        math_basics/function.cpp
        math_basics/map.h
        math_basics/map.cpp
        math_basics/parametr.h
        math_basics/parametr.cpp
        matbase.cpp
        matbase.h
        modyfikatory.cpp
        modyfikatory.h
        obraz.cpp
        obraz.h)

target_link_libraries(qtknots Qt5::Widgets)
qt5_use_modules(qtknots Widgets OpenGL)
