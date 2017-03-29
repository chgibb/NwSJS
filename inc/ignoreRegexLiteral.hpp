#pragma once
#include <string>
#include "StreamBuffer.hpp"
namespace nwsjs
{
    void ignoreRegexLiteral(
        nwsjs::StreamPassBuffer&buff,
        unsigned int&i,
        size_t end
    )
    {
        if(buff.bytes[i].byte == '/' && buff.bytes[i+1] != '/')
        {
            while(i != end)
            {
                i++;
                if(buff.bytes[i].byte == '\n')
                {
                    i++;
                    return;
                }
            }
        }
    }
}