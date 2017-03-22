 #!/bin/bash
(set -o igncr) 2>/dev/null && set -o igncr; # For Cygwin on Windows compaibility
rm nwsjs

CXX="g++"
if [ "$TRAVIS" = true ]; then 
    CXX="/usr/bin/g++-5"
    printf "Detected TravisCI\n"
fi


echo $PATH
if [ "$1" = "APPVEYOR" ]; then
    export PATH=$PATH:/c/mingw/bin
    export PATH=/usr/bin:$PATH
    printf "Detected AppVeyor\n"
fi

$CXX --version
printf "Building nwsjs\n"

$CXX -c -Wall -fexceptions -fexpensive-optimizations -O3 -std=c++11 -g main.cpp > log
$CXX  -o nwsjs main.o  -s > log
rm *.o
