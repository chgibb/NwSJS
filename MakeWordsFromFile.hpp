/*
	Author: Chris Gibb
    Student I.D.: 0567901
    Contact: cgibb@lakeheadu.ca
    Instructor: Dr. Sabah Mohammed
    Course: CS 2430
    Modified From Original By Chris Gibb to Include C-Style Comment Stripping
*/
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
//parse file identified by string filename into individual words
//held in tokenlist
bool MakeWordsFromFile(std::string filename,std::vector<std::string>&tokenlist,bool lowercase = true)
{
    std::ifstream file(filename.c_str(),std::ios::in);
    char byte;
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
                file.get(byte);
                //single line
                if(byte == '/')
                {
                    //consume the line
                    while(byte != '\n')
                    {
                        file.get(byte);
                    }
                    break;
                }
                //multi line
                else if(byte == '*')
                {
                    for(;;)
                    {
                        file.get(byte);
                        if(byte == '*')
                        {
                            file.get(byte);
                            if(byte == '/')
                            {
                                file.get(byte);
                                break;
                            }
                        }
                    }
                    break;
                }
                else
                {
                    str += "/";
                    str += byte;
                    file.get(byte);
                }
            break;
            case ' ':
                //cull common punctuation and formating characters
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case '(':
                //cull common punctuation and formating characters
                str += "(";
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case ')':
                //cull common punctuation and formating characters
                str += ")";
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case '{':
                //cull common punctuation and formating characters
                str += "{";
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case '}':
                //cull common punctuation and formating characters
                str += "}";
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case ',':
                //cull common punctuation and formating characters
                str += ",";
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                    tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case '\n':
                //cull common punctuation and formating characters
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                tokenlist.push_back(str);
                str = "";
                add = false;
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
        if(add && byte != '\t' && byte != '\'' && byte != '\"')
            str += byte;
    }
	file.close();
    return true;
}
