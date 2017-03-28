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
        else if(token == "in" || token == "instanceof" || token == "extends")
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
        int i = 1;
        while(file.get(byte))
        {
            add = true;
            switch(byte)
            {
                case '/':
                    if(nwsjs::options::comments)
                    {
                        file.get(byte);
                        if(!byte)
                            return true;
                        //single line
                        if(byte == '/')
                        {
                            i = 1;
                            //consume the line
                            for(;;)
                            {
                                //std::cerr<<"trying to consume single line comment \""<<byte<<"\""<<i<<"\n";
                                if(byte == '\n')
                                    break;
                                file.get(byte);
                                if(!byte)
                                    return true;    
                                ++i;
                            }
                            stream<<"\n";
                            break;
                        }
                        //multi line
                        else if(byte == '*')
                        {
                            i = 1;
                            for(;;)
                            {
                                //std::cerr<<"trying to consume multi line comment \""<<byte<<"\""<<i<<"\n";
                                file.get(byte);
                                if(lastByte == '*' && byte == '/')
                                {
                                    
                                    file.get(byte);
                                    if(!byte)
                                        return true;    
                                    break;
                                }
                                ++i;
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
                                if(!byte)
                                    return true;    
                            }
                        }
                    }
                break;
                case '\"':
                    str += byte;
                    i = 1;
                    for(;;)
                    {
                        //std::cerr<<"trying to consume double quoted string \""<<byte<<"\""<<i<<"\n";
                        file.get(byte);
                        if(!byte)
                            return true;    
                        str += byte;
                        if(byte == '\"' && lastByte != '\\')
                            break;
                        lastByte = byte;
                        ++i;
                    }
                break;
                case '\'':
                    str += byte;
                    i = 1;
                    for(;;)
                    {
                        //std::cerr<<"trying to consume single quoted string \""<<byte<<"\""<<i<<"\n";
                        file.get(byte);
                        if(!byte)
                            return true;    
                        str += byte;
                        if(byte == '\'' && lastByte != '\\')
                            break;
                        lastByte = byte;
                        i++;
                    }
                break;
                case '`':
                    str += byte;
                    i = 1;
                    for(;;)
                    {
                        //std::cerr<<"trying to consume backtick quoted string \""<<byte<<"\""<<i<<"\n";
                        file.get(byte);
                        if(!byte || !file)
                        {
                            stream<<str;
                            return true;
                        }
                        str += byte;
                        if(byte == '`')
                            break;
                        ++i;
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
        for(unsigned int i = 0; i != buff.bytes.size(); ++i)
        {
            if((buff.bytes[i].byte == ';' || buff.bytes[i].byte == '\n' || buff.bytes[i].byte == '{') && buff.bytes[i + 1].byte == '\n')
                buff.bytes[i + 1].stream = false;

            if((buff.bytes[i].byte == ' ') && buff.bytes[i + 1].byte == ' ')
                buff.bytes[i + 1].stream = false;
        }
    }
}
