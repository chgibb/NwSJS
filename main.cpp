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
    std::vector<std::string> args;
    for(int i = 0; i != argc; i++)
    {
        args.push_back(argv[i]);
    }
    int parseOptions = 0;
    for(auto it = args.begin(); it != args.end(); ++it)
    {
        if(*it == "--comments")
            nwsjs::options::comments = true;
        if(*it == "--spaces")
            nwsjs::options::spaces = true;
        if(*it == "--tabs")
            nwsjs::options::tabs = true;
        if(*it == "--newLines")
            nwsjs::options::newLines = true;
    }
    if(!nwsjs::options::newLines)
    {
        if(!nwsjs::tokenizeAndCompress<decltype(std::cout)>(std::string(argv[1]),std::cout))
        {
            std::cout<<"Could not open "<<argv[1]<<"\n";
            return 1;
        }
    }
    else if(parseOptions&nwsjs::options::newLines)
    {
        nwsjs::StreamPassBuffer passBuff;
        /*if(!nwsjs::tokenizeAndCompress<decltype(passBuff)>(std::string(argv[1]),parseOptions,passBuff))
        {
            std::cout<<"Could not open "<<argv[1]<<"\n";
            return 1;
        }*/
        nwsjs::tokenizeAndCompress<decltype(passBuff)>(std::string(argv[1]),passBuff);
        nwsjs::secondPassCompression(passBuff);
        for(int i = 0; i != passBuff.bytes.size() - 1; ++i)
        {
            if(passBuff.bytes[i].stream)
                std::cout<<passBuff.bytes[i].byte;
        }
    }
    return 0;
}
