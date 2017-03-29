#pragma once
#include <string>
#include "StreamBuffer.hpp"
#include "ignoreWrappedSequence.hpp"
#include "ignoreRegexLiteral.hpp"
namespace nwsjs
{
    void stripComments(nwsjs::StreamPassBuffer&buff)
    {
        size_t end = buff.bytes.size();
        for(unsigned int i = 0; i != end; ++i)
        {
            nwsjs::ignoreRegexLiteral(buff,i,end);
            if(buff.bytes[i].byte == '/' && buff.bytes[i+1].byte == '/')
            {
                buff.bytes[i].stream = false;
                buff.bytes[i+1].stream = false;
                i++;
                i++;
                while(i != end)
                {
                    if(buff.bytes[i].byte == '\n')
                    {
                        break;
                    }
                    buff.bytes[i].stream = false;
                    i++;
                }
            }
            if(buff.bytes[i].byte == '/' && buff.bytes[i+1].byte == '*')
            {
                buff.bytes[i].stream = false;
                buff.bytes[i+1].stream = false;
                i++;
                i++;
                while(i != end)
                {
                    if(buff.bytes[i].byte == '*' && buff.bytes[i+1].byte == '/')
                    {
                        buff.bytes[i].stream = false;
                        buff.bytes[i+1].stream = false;
                        i++;
                        i++;
                        break;
                    }
                    buff.bytes[i].stream = false;
                    i++;
                }
            }
            nwsjs::ignoreWrappedSequence(buff,i,end,'\"');
            nwsjs::ignoreWrappedSequence(buff,i,end,'\'');
            nwsjs::ignoreWrappedSequence(buff,i,end,'`');
        }
    }
}