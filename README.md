# qtknots
Program for computing knots, crossings and double negative index from C-link.

REQUIRED PACKAGES:
- pkg-config
- qt5
- gsl
- freeglut
- graphviz
- python (2.7)
- pip
- python-tk (only linux)
- virtualenv (from pip)

 COMMAND FOR LINUX
```
sudo apt install make pkg-config build-essential qt5-default libgsl0-dev freeglut3-dev graphviz python2.7 python-pip python-tk
pip2 install virtualenv
```
 COMMAND FOR MAC OX
```
brew install pkg-config python@2 qt5 freeglut graphviz 
brew link qt5 --force
pip2 install virtualenv
```
TO INSTALL, CALL:
```
./qtknots/install.sh
```
TO RUN THE PROGRAM, CALL:
```
./qtknots/run.sh
```
TO CLEAN AFTER INSTALLATION, CALL:
```
./qtknots/clean.sh
```

THE PROGRAM ALLOWS TO INPUT FUNCTION FROM THE FILE. THE SAMPLE FILE IS INCLUDED - PLEASE FOLLOW THE FORMAT:
```
name_of_function_without_spaces
power_x power_y real imag 
```
