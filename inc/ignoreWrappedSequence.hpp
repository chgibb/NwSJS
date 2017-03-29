#pragma once
#include <string>
#include "StreamBuffer.hpp"
namespace nwsjs
{
    std::string ignoreWrappedSequence(
        nwsjs::StreamPassBuffer&buff,
        unsigned int&i,
        size_t end,
        char ignore
    )
    {
        if(buff.bytes[i].byte == ignore)
        {
            std::string str = "";
            while(i != end)
            {
                str += buff.bytes[i].byte;
                if(buff.bytes[i].byte == ignore && buff.bytes[i].byte != '\\')
                {
                    i++;
                    return str;
                }
                i++;
            }
        }
        return "";
    }
}