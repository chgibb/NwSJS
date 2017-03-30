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
            str += ignore;
            while(i < end)
            {
                i++;
                str += buff.bytes[i].byte;
                if(buff.bytes[i].byte == ignore && buff.bytes[i-1].byte != '\\')
                {
                    i++;
                    return str;
                }
            }
        }
        return "";
    }
}