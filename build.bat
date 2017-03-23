set path=%path%;C:\mingw\bin
g++ -c -Wall -fexceptions -fexpensive-optimizations -O3 -std=c++11 -static -g main.cpp
g++ -static -static-libgcc -static-libstdc++ -o nwsjs main.o  -s
