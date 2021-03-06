######################################################################
# Automatically generated by qmake (3.1) Tue Nov 21 21:03:42 2017
######################################################################

TEMPLATE = app
TARGET = qtknots
CONFIG += c++14 debug

# pkg-config
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig

PKGCONFIG += gsl
mac {
  PKGCONFIG += freeglut
  QMAKE_CXX = c++
  QMAKE_CC = cc
  QMAKE_LINK = c++
}
unix:!mac {
  PKGCONFIG += glu
}

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/math_basics/sample-func.h \
           src/math_basics/function.h \
           src/GLpart.h \
           src/math_basics/fourvector.h \
           src/math_basics/matrix.h \
           src/math_basics/map.h \
           src/math_basics/parametr.h \
           src/math_basics/matbase.h \
           src/modyfikatory.h \
           src/obraz.h \
           src/log.h \
           src/Knot.h \
           src/crossings.h \
           src/graphs.h
SOURCES += src/math_basics/sample-func.cpp \
           src/math_basics/function.cpp \
           src/GLpart.cpp \
           src/main.cpp \
           src/math_basics/fourvector.cpp \
           src/math_basics/matrix.cpp \
           src/math_basics/map.cpp \
           src/math_basics/parametr.cpp \
           src/math_basics/matbase.cpp \
           src/modyfikatory.cpp \
           src/obraz.cpp \
           src/Knot.cpp \
           src/crossings.cpp \
           src/log.cpp \
           src/graphs.cpp
QT += opengl

OBJECTS_DIR=build #Intermediate object files directory
MOC_DIR=build #Intermediate moc files directory
