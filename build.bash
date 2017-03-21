rm nwsjs
g++ --version
printf "Building nwsjs\n"

g++ -Wall -fexceptions -fexpensive-optimizations -O3 -std=c++11  -c main.cpp -o main.o
g++  -o nwsjs main.o  -s  
rm *.o
