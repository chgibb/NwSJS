#pragma once
#include <string>
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
                        buff.bytes[i].stream = false;
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
            
            /*if(buff.bytes[i].byte == '/')
            {
                if(nwsjs::options::comments)
                {
                    i++;
                    if(i == end)
                        return;
                    if(buff.bytes[i].byte == '/')
                    {
                        buff.bytes[i-1].stream = false;
                        while(i != end)
                        {
                            if(buff.bytes[i].byte == '\n')
                            {
                                buff.bytes[i].stream = false;
                                break;
                            }
                            buff.bytes[i].stream = false;
                            i++;
                        }
                    }
                }
            }*/
        }
    }
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
            if(byte == '+')
            {
                if(str != "")
                {
                    stream<<str;
                    stream<<byte;
                    str = "";
                    continue;
                }
                else
                {
                    stream<<"+";
                    continue;
                }
            }
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
                                file.get(byte);
                                if(!byte || !file)
                                    return true;  
                                //std::cerr<<"trying to consume single line comment \""<<byte<<"\""<<i<<"\n";
                                if(byte == '\n')
                                    break;
                                  
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
                        if(!byte || !file)
                        {
                            stream<<str;
                            return true;
                        }
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
                        if(!byte || !file)
                        {
                            stream<<str;
                            return true;
                        }
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
                else if(byte == '\t')
                {
                    if((nwsjs::options::tabs) == 0)
                        str += "\t";
                    if(str != "")
                        stream<<nwsjs::addWhiteSpaceToToken(str);
                    str = "";
                    add = false;
                    break;
                }
                else if(byte == *it)
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
}