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
    nwsjs::StreamPassBuffer buff;
    bool res = nwsjs::bufferFile(argv[1],buff);
    if(nwsjs::options::comments)
        nwsjs::stripComments(buff);
    nwsjs::stripWhiteSpace(buff);
    if(!res)
    {
        std::cerr<<"Could not open "<<argv[1]<<std::endl;
    }
    buff.print(std::cout);
    return 0;
}
