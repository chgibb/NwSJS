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
            parseOptions |= nwsjs::options::comments;
        if(*it == "--spaces")
            parseOptions |= nwsjs::options::spaces;
        if(*it == "--tabs")
            parseOptions |= nwsjs::options::tabs;
        if(*it == "--newLines")
            parseOptions |= nwsjs::options::newLines;
    }
    if((parseOptions&nwsjs::options::newLines) == 0)
    {
        if(!nwsjs::tokenizeAndCompress<decltype(std::cout)>(std::string(argv[1]),parseOptions,std::cout))
        {
            std::cout<<"Could not open "<<argv[1]<<"\n";
            return 1;
        }
    }
    else if(parseOptions&nwsjs::options::newLines)
    {
        nwsjs::StreamPassBuffer passBuff;
        if(!nwsjs::tokenizeAndCompress<decltype(passBuff)>(std::string(argv[1]),parseOptions,passBuff))
        {
            std::cout<<"Could not open "<<argv[1]<<"\n";
            return 1;
        }
    }
    return 0;
}
