#pragma once
#include <fstream>
#include "StreamBuffer.hpp"
namespace nwsjs
{
    bool bufferFile(char*path,nwsjs::StreamPassBuffer&buff)
    {
        std::ifstream file(path,std::ios::in);
        if(file.fail())
		    return false;
        char byte;
        while(file.get(byte))
        {
            buff<<byte;
        }
        return true;
    }
}