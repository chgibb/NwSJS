#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
namespace nwsjs
{
    std::string addWhiteSpaceToToken(std::string&token)
    {
        if(token == "var" || token == "function" || token == "return" ||
        token == "new" || token == "else" || token == "typeof" || token == "class" ||
        token == "throw" || token == "let" || token == "const" || token == "await" || 
        token == "async" || token == "yield" || token == "break" || token == "continue" ||
        token == "case" || token == "void")
            return token + " ";
        else if(token == "in" || token == "instanceof")
            return " "+token+" ";
        return token;
    }
    void stripChar(std::string&str,char&c)
    {
        str.erase(std::remove(str.begin(),str.end(),c),str.end());
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
        ',',';',':',
        '=','+','-','*','/','|','&','?',
        '\n'
    };
    auto delimTokensEnd = delimTokens.end();

    template<class T>
    bool tokenizeAndCompress(std::string filename,T&stream)
    {
        std::ifstream file(filename.c_str(),std::ios::in);
        char byte;
        char lastByte = ';';
        std::string str;
        bool add = true;
        if(file.fail())
		    return false;
        while(file.get(byte))
        {
            add = true;
            switch(byte)
            {
                case '/':
                    if(nwsjs::options::comments)
                    {
                        file.get(byte);
                        //single line
                        if(byte == '/')
                        {
                            //consume the line
                            for(;;)
                            {
                                if(byte == '\n')
                                    break;
                                file.get(byte);
                            }
                            stream<<"\n";
                            break;
                        }
                        //multi line
                        else if(byte == '*')
                        {
                            for(;;)
                            {
                                file.get(byte);
                                if(lastByte == '*' && byte == '/')
                                {
                                    file.get(byte);
                                    break;
                                }
                                lastByte = byte;
                            }
                            break;
                        }
                        else
                        {
                            str += "/";
                            for(;;)
                            {
                                str += byte;
                                if(byte == '\n')
                                    break;
                                file.get(byte);
                            }
                        }
                    }
                break;
                case '\"':
                    str += byte;
                    for(;;)
                    {
                        file.get(byte);
                        str += byte;
                        if(byte == '\"' && lastByte != '\\')
                            break;
                        lastByte = byte;
                    }
                break;
                case '\'':
                    str += byte;
                    for(;;)
                    {
                        file.get(byte);
                        str += byte;
                        if(byte == '\'' && lastByte != '\\')
                            break;
                        lastByte = byte;
                    }
                break;
                case '`':
                    str += byte;
                    for(;;)
                    {
                        file.get(byte);
                        str += byte;
                        if(byte == '`')
                            break;
                    }
                    continue;
                break;
            }
            if(lastByte == ' ' && byte == '+')
            {
                char nextByte;
                file.get(nextByte);
                if(nextByte != '+')
                {
                    stream<<lastByte;
                    stream<<byte;
                    str += nextByte;
                }
                continue;
            }
            for(auto it = nwsjs::delimTokens.begin(); it != nwsjs::delimTokensEnd; ++it)
            {
                if(byte == ' ')
                {
                    if((nwsjs::options::spaces) == 0)
                        str += " ";
                    if(str != "")
                        stream<<nwsjs::addWhiteSpaceToToken(str);
                    str = "";
                    add = false;
                    break;
                }
                if(byte == '\t')
                {
                    if((nwsjs::options::tabs) == 0)
                        str += "\t";
                    if(str != "")
                        stream<<nwsjs::addWhiteSpaceToToken(str);
                    str = "";
                    add = false;
                    break;
                }
                if(byte == *it)
                {
                    str += *it;
                    if(str != "")
                        stream<<nwsjs::addWhiteSpaceToToken(str);
                    str = "";
                    add = false;
                    break;
                }
            }
            if(add && byte != '\'' && byte != '\"')
                str += byte;

            lastByte = byte;
        }
	    file.close();
        return true;
    }
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

    void secondPassCompression(StreamPassBuffer&buff)
    {
        for(int i = 0; i != buff.bytes.size() - 1; ++i)
        {
            if((buff.bytes[i].byte == ';' || buff.bytes[i].byte == '\n') && buff.bytes[i + 1].byte == '\n')
                buff.bytes[i + 1].stream = false;
        }
    }
}
