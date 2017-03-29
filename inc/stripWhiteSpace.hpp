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
                nwsjs::ignoreRegexLiteral(buff,i,end);
                nwsjs::ignoreWrappedSequence(buff,i,end,'\"');
                nwsjs::ignoreWrappedSequence(buff,i,end,'\'');
                nwsjs::ignoreWrappedSequence(buff,i,end,'`');
                for(auto it = delimTokens.begin(); it != delimTokensEnd; ++it)
                {
                    if(buff.bytes[i].byte == *it)
                    {
                        if(buff.bytes[i+1].stream)
                        {
                            if(options::spaces)
                            {
                                if(buff.bytes[i+1].byte == ' ')
                                {
                                    buff.bytes[i+1].stream = false;
                                    break;
                                }
                            }
                            if(options::tabs)
                            {
                                if(buff.bytes[i+1].byte == '\t')
                                {
                                    buff.bytes[i+1].stream = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}