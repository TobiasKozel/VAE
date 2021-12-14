#!/bin/sh
g++ -g ./runtime.cpp
./a.out
g++ -g ./compiletime.cpp -lvae -L../../build/ -Wl,-rpath,../../build/
./a.out