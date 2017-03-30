#pragma once
#include <string>
#include "StreamBuffer.hpp"
#include "ignoreWrappedSequence.hpp"
#include "ignoreRegexLiteral.hpp"
namespace nwsjs
{
    nwsjs::StreamPassBuffer stripNewLines(nwsjs::StreamPassBuffer buff)
    {
        size_t end = buff.bytes.size();
        for(unsigned int i = 0; i < end; ++i)
        {
            if(buff.bytes[i].stream)
            {
                nwsjs::ignoreRegexLiteral(buff,i,end);
                nwsjs::ignoreWrappedSequence(buff,i,end,'\"');
                nwsjs::ignoreWrappedSequence(buff,i,end,'\'');
                nwsjs::ignoreWrappedSequence(buff,i,end,'`');
                if(buff.bytes[i].byte == '\r')
                    buff.bytes[i].stream = false;
            }
        }
        for(unsigned int i = 0; i < end; ++i)
        {
            if(buff.bytes[i].stream)
            {
                nwsjs::ignoreRegexLiteral(buff,i,end);
                nwsjs::ignoreWrappedSequence(buff,i,end,'\"');
                nwsjs::ignoreWrappedSequence(buff,i,end,'\'');
                nwsjs::ignoreWrappedSequence(buff,i,end,'`');
                if((buff.bytes[i].byte == ';' || buff.bytes[i].byte == ',' || buff.bytes[i].byte == '\n' || buff.bytes[i].byte == '{') && buff.bytes[i + 1].byte == '\n')
                    buff.bytes[i + 1].stream = false;

                if(buff.bytes[i].byte == '\n' && buff.bytes[i + 1].byte == '}')
                    buff.bytes[i].stream = false;
            }
        }
        nwsjs::StreamPassBuffer res;
        buff.print(res);
        return res;
    }
}