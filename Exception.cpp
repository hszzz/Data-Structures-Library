#include "Exception.h"
#include <string.h>
#include <cstdlib>
using namespace std;
namespace HsTL
{
void Exception::init(const char* message,const char* file,int line)
{
    m_message = (char*)malloc(strlen(message)+1);
    strcpy(m_message,message);
    if(file != NULL)
    {
        //将行号转换称字符串，并和file拼接
        char tmpLine[16]={0};
        itoa(line,tmpLine,10);
        m_location=(char*)malloc(strlen(file)+strlen(tmpLine)+2);
        if(m_location != NULL)
        {
            strcpy(m_location,file);
            strcat(m_location,":");
            strcat(m_location,tmpLine);
        }
    }
    else
    {
        m_location = NULL;
    }
}

Exception::Exception(const char* message)
{
    init(message,NULL,0);
}

Exception::Exception(const char* file,int line)
{
    init(NULL,file,line);
}

Exception::Exception(const char* message,const char* file,int line)
{
    init(message,file,line);
}

Exception::Exception(const Exception &e)
{
    m_message=(char*)malloc(strlen(e.m_message)+1);
    strcpy(m_message,e.m_message);
    m_location=(char*)malloc(strlen(e.m_location)+1);
    strcpy(m_location,e.m_location);
}

Exception& Exception::operator=(const Exception& e)
{
    if(this != &e)
    {
        free(m_message);
        free(m_location);

        m_message=(char*)malloc(strlen(e.m_message)+1);
        strcpy(m_message,e.m_message);
        m_location=(char*)malloc(strlen(e.m_location)+1);
        strcpy(m_location,e.m_location);
    }
    return *this;
}

const char* Exception::message() const
{
    return this->m_message;
}

const char* Exception::location() const
{
    return this->m_location;
}

Exception::~Exception()
{
    free(m_message);
    free(m_location);
}

}
