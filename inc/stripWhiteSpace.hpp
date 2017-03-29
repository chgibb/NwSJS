#pragma once
#include <string>
#include "StreamBuffer.hpp"
#include "ignoreWrappedSequence.hpp"
#include "ignoreRegexLiteral.hpp"
namespace nwsjs
{
    void stripWhiteSpace(nwsjs::StreamPassBuffer&buff)
    {
        size_t end = buff.bytes.size();
        for(unsigned int i = 0; i != end; ++i)
        {
            if(buff.bytes[i].stream)
            {
                
            }
        }
    }
}