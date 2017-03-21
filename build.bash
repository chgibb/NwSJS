rm nwsjs

CXX="g++"
if [ "$travis" = true ]; then 
    CXX="g++-5.4"
    printf "Detected TravisCI\n"
fi

$CXX --version
printf "Building nwsjs\n"

$CXX -Wall -fexceptions -fexpensive-optimizations -O3 -std=c++11  -c main.cpp -o main.o
$CXX  -o nwsjs main.o  -s  
rm *.o
