 #!/bin/bash
(set -o igncr) 2>/dev/null && set -o igncr; # For Cygwin on Windows compaibility
rm nwsjs

CXX="g++"
if [ "$TRAVIS" = true ]; then 
    CXX="/usr/bin/g++-5"
    printf "Detected TravisCI\n"
fi

if [ "$1" = "APPVEYOR" ]; then
    CXX="C:\\mingw\\bin\\g++"
    printf "Detected AppVeyor\n"
fi

$CXX --version
printf "Building nwsjs\n"

$CXX -Wall -fexceptions -fexpensive-optimizations -O3 -std=c++11  -c main.cpp -o main.o
$CXX  -o nwsjs main.o  -s  
rm *.o
