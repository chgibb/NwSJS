#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
namespace nwsjs
{
    std::string addWhiteSpaceToToken(std::string&token)
    {
        std::string tmp = token;
        token = "";
        for(int i = 0; i != tmp.length(); ++i)
        {
            if(tmp[i] == '+')
            {
                if(i != 0)
                {
                    if(tmp[i - 1] == ' ')
                    {
                        token += ' ';
                        token += tmp[i];
                        continue;
                    }
                }
            }
            token += tmp[i];
        }
        //fprintf(stderr,"token: \"%s\"\n",token.c_str());
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
        int comments = 0x01;
        int spaces = 0x02;
        int tabs = 0x04;
        int tokensToStdErr = 0x08;
    }
    std::vector<char> delimTokens{
        '(',')',
        '{','}',
        ',',';',':',
        '=','+','-','*','/','|','&',
        '\n'
    };
    auto delimTokensEnd = delimTokens.end();

    template<class T>
    bool tokenizeJS(std::string filename,int&parseOptions,T&stream)
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
                    if(parseOptions&nwsjs::options::comments)
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
                        if(byte == '\"')
                            break;
                    }
                break;
                case '\'':
                    str += byte;
                    for(;;)
                    {
                        file.get(byte);
                        str += byte;
                        if(byte == '\'')
                            break;
                    }
                break;
            }
            if(lastByte == ' ' && byte == '+')
            {
                stream<<lastByte;
                stream<<byte;
                continue;
            }
            for(auto it = nwsjs::delimTokens.begin(); it != nwsjs::delimTokensEnd; ++it)
            {
                if(byte == ' ')
                {
                    if((parseOptions&nwsjs::options::spaces) == 0)
                        str += " ";
                    if(str != "")
                        stream<<nwsjs::addWhiteSpaceToToken(str);
                    str = "";
                    add = false;
                    break;
                }
                if(byte == '\t')
                {
                    if((parseOptions&nwsjs::options::tabs) == 0)
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
}
