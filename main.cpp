#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "inc/NwSJS.hpp"
using namespace std;
int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        std::cout<<"Insufficient arguments";
        return 1;
    }
    nwsjs::StreamPassBuffer buff;
    bool res = nwsjs::bufferFile(argv[1],buff);
    if(!res)
    {
        std::cerr<<"Could not open "<<argv[1]<<std::endl;
    }
    if(buff.bytes.size() == 0)
    {
        return 0;
    }
    buff = nwsjs::stripNewLines(
        nwsjs::stripWhiteSpace(
            nwsjs::stripComments(buff)
        )
    );
    buff.print(std::cout);
    return 0;
}
