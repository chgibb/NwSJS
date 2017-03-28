#pragma once
#include <vector>
#include <string>
namespace nwsjs
{
    struct StreamByteBuffer
    {
        char byte;
        bool stream;
    };
    class StreamPassBuffer
    {
        public:
            StreamPassBuffer() = default;
            ~StreamPassBuffer() = default;
            std::vector<StreamByteBuffer> bytes;
    };
    StreamPassBuffer&operator<<(StreamPassBuffer&buff,std::string str)
    {
        for(unsigned int i = 0; i != str.length(); ++i)
        {
            struct StreamByteBuffer byteBuff;
            byteBuff.byte = str[i];
            byteBuff.stream = true;
            buff.bytes.push_back(byteBuff);
        }
        return buff;
    }
    StreamPassBuffer&operator<<(StreamPassBuffer&buff,char&byte)
    {
        struct StreamByteBuffer byteBuff;
        byteBuff.byte = byte;
        byteBuff.stream = true;
        buff.bytes.push_back(byteBuff);
        return buff;
    }
}