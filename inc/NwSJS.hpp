#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "StreamBuffer.hpp"
#include "bufferFile.hpp"
namespace nwsjs
{
    std::string addWhiteSpaceToToken(std::string&token)
    {
        ////std::cerr<<"\""<<token<<"\""<<std::endl;
        if(token == "var" || token == "function" || token == "return" ||
        token == "new" || token == "else" || token == "typeof" || token == "class" ||
        token == "throw" || token == "let" || token == "const" || token == "await" || 
        token == "async" || token == "yield" || token == "break" || token == "continue" ||
        token == "case" || token == "void")
            return token + " ";
        else if(token == "in" || token == "instanceof" || token == "extends")
            return " "+token+" ";
        return token;
    }

    namespace options
    {
        bool comments = false;
        bool spaces = false;
        bool tabs = false;
        bool newLines = false;

    }
    std::vector<char> delimTokens{
        '(',')',
        '{','}',
        ',',';',':',' ',
        '=','-','*','/','|','&','?',
        '\n'
    };
    auto delimTokensEnd = delimTokens.end();

    void secondPassCompression(StreamPassBuffer&buff)
    {
        for(unsigned int i = 0; i != buff.bytes.size(); ++i)
        {
            if((buff.bytes[i].byte == ';' || buff.bytes[i].byte == '\n' || buff.bytes[i].byte == '{') && buff.bytes[i + 1].byte == '\n')
                buff.bytes[i + 1].stream = false;

            if((buff.bytes[i].byte == ' ') && buff.bytes[i + 1].byte == ' ')
                buff.bytes[i + 1].stream = false;
        }
    }
}
#include "lexAndCompress.hpp"
