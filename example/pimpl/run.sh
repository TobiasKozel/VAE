#!/bin/sh
g++ -g ./main.cpp -lvae -L../../build/ -Wl,-rpath,../../build/
./a.out