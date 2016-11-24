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
    std::vector<std::string> tokens;
    if(!::tokenizeJS(std::string(argv[1]),tokens))
    {
        std::cout<<"Could not open "<<argv[1]<<"\n";
        return 1;
    }
    auto end = tokens.end();
    for(auto it = tokens.begin(); it != end; ++it)
    {
        if(*it == "var" || *it == "function" || *it == "return" ||
         *it == "new" || *it == "else" || *it == "typeof" || *it == "class" ||
         *it == "throw" || *it == "let" || *it == "const" || *it == "await" || 
		 *it == "async" || *it == "yield" || *it == "break" || *it == "continue" ||
		 *it == "case")
            std::cout<<*it<<" ";
        else
            std::cout<<*it;
    }
    return 0;
}
