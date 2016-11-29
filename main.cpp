/*
    Extremely simple, lightweight CLI utility to strip whitespace and comments from Javascript source code.
	Approximately 75x faster, with 25% less CPU usage than UglifyJS at the same task. Tends to choke on files > 500kb however.
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "inc/tokenizeJS.hpp"
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
    }
    std::vector<std::string> tokens;
    if(!::tokenizeJS<decltype(std::cout)>(std::string(argv[1]),parseOptions,std::cout))
    {
        std::cout<<"Could not open "<<argv[1]<<"\n";
        return 1;
    }
    return 0;
}
