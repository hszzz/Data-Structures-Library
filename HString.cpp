#include <cstring>
#include <cstdlib>
#include "HString.h"
#include "Exception.h"
using namespace std;

namespace HsTL
{
//部分匹配表
int* String::make_pmt(const char* s)
{
    int len = strlen(s);

    int* ret = static_cast<int*>(malloc(sizeof(int) * len));

    if(ret != NULL)
    {
        int ll = 0;//前缀，后缀最大交集元素的长度
        ret[0] = 0;
        for(int i=1; i<len; i++)
        {
            while((s[ll] != s[i]) && (ll > 0))//在前一个ll上进行扩展，比对最后扩展的元素是否相等,不成功则继续尝试扩展
            {                                 //当ll = 0的时候结束循环
                ll = ret[ll - 1];
            }

            if(s[ll] == s[i])//在前一个ll上进行扩展，比对最后扩展的元素是否相等，相等则 + 1（最理想情况下）
            {
                ll++;
            }
            ret[i] = ll;
        }
    }
    else
    {

    }

    return ret;
}


//KMP算法
int String::kmp(const char* s/*被查找的串*/, const char* p/*需要查找的串*/)
{
    int ret = -1;
    int sl = strlen(s);
    int pl = strlen(p);
    int* pmt = make_pmt(p);

    if((pmt != NULL) && (0 < pl) && (pl <= sl))
    {
        for(int i=0, j=0; i<sl; i++)
        {
            while ((j > 0) && (s[i] != p[j])) //匹配不成功就查 PMT表
            {                                 //右移的位置为 pmt[j-1]
                j = pmt[j-1];
            }

            if(s[i] == p[j]) //当比对的字符符合时，再比对下一个
            {
                j++;
            }

            if(j == pl)      //当 j 就是 子串 的长度，意味着匹配成功
            {
                ret = i + 1 - pl; //子串 在 被查找的串 中的位置
                break;
            }
        }
    }
    free(pmt);

    return ret;
}

void String::init(const char *s)
{
    m_str = strdup(s);
    if(m_str)
    {
        m_length = strlen(m_str);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To Create string ...");
    }
}

bool String::equal(const char *l, const char *r, int len) const
{
    bool ret = true;

    for(int i=0; i<len && ret; i++)
    {
        ret = ret && (l[i] == r[i]);//如果有一个不对，就结束循环
    }

    return ret;
}

String::String()
{
    init("");
}

String::String(const char* s)
{
    init(s ? s : "");
}

String::String(char c)
{
    char tmp[] = {c, '\0'};
    init(tmp);
}

String::String(const String &s)
{
    init(s.m_str);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}


bool String::operator == (const String& e) const
{
    return (strcmp(m_str, e.m_str) == 0);
}

bool String::operator == (const char* s) const
{
    return (strcmp(m_str, s ? s : "") == 0);
}

bool String::operator != (const String& e) const
{
    return !(*this == e);
}

bool String::operator != (const char* s) const
{
    return !(*this == s);
}

bool String::operator > (const String& e) const
{
    return (strcmp(m_str, e.m_str) > 0);
}

bool String::operator > (const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator < (const String& e) const
{
    return (strcmp(m_str, e.m_str) < 0);
}

bool String::operator < (const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator >= (const String& e) const
{
    return (strcmp(m_str, e.m_str) >= 0);
}

bool String::operator >= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator <= (const String& e) const
{
    return (strcmp(m_str, e.m_str) <= 0);
}

bool String::operator <= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}


String String::operator + (const String& e) const
{
    return (*this + e.m_str);
}

String String::operator + (const char* s) const
{
    String ret;
    int len = m_length + strlen(s ? s : "");

    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if(str)
    {
        strcpy(str, m_str);
        strcat(str, s ? s : "");

        free(ret.m_str);
        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To add String values ...");
    }

    return ret;
}

String& String::operator += (const String& e)
{
    return (*this = *this + e.m_str);
}

String& String::operator += (const char* s)
{
    return (*this = *this + s);
}

String String::operator - (const String& e) const
{
    return String(*this).remove(e);
}

String String::operator - (const char* s) const
{
    return String(*this).remove(s);
}

String& String::operator -= (const String& e)
{
    return remove(e);
}

String& String::operator -= (const char* s)
{
    return remove(s);
}

String& String::operator = (const String& e)
{
    return (*this = e.m_str);
}

String& String::operator = (const char* s)
{
    if(m_str != s)
    {
        char* str = strdup(s ? s : "");

        if(str)
        {
            free(m_str);

            m_str = str;
            m_length = strlen(m_str);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To Assign New String Value ...");
        }
    }

    return *this;
}

String& String::operator = (char c)
{
    char tmp[] = {c, '\0'};
    return (*this = tmp);
}

char& String::operator [] (int index)
{
    if((0 <= index) && (index < m_length))
    {
        return m_str[index];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter index Is Invalid ...");
    }
}

char String::operator [] (int index) const
{
    return (const_cast<String&>(*this))[index];
}

//判断字符串是否以指定的字符串开始或结束
bool String::start_with(const char* s) const
{
    bool ret = (s != NULL);

    if(ret)
    {
        int len = strlen(s);
        ret = (len < m_length) && equal(m_str, s, len);
    }

    return ret;
}

bool String::start_with(const String& e) const
{
    return start_with(e.m_str);
}

bool String::end_of(const char* s) const
{
    bool ret = (s != NULL);

    if(ret)
    {
        int len = strlen(s);
        char* str = m_str + (m_length - len);//定位到结尾部分
        ret = (len < m_length) && equal(str, s, len);
    }

    return ret;
}

bool String::end_of(const String& e) const
{
    return end_of(e.m_str);
}

//在指定位置插入一个字符串
String& String::insert(int index, const char* s)
{
    if((0 <= index) && (index <= m_length))
    {
        if((s != NULL) && (s[0] != '\0'))
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len +1));

            if(str != NULL)
            {
                strncpy(str, m_str, index);//copy指定长度的字符串
                strncpy(str + index, s, len);
                strncpy(str + index +len, m_str, m_length - index);
                str[m_length + len] = '\0';

                free(m_str);
                m_str = str;
                m_length = m_length + len;

            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To insert String ...");
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter index Is Invalid ...");
    }

    return *this;
}

String& String::insert(int index, const String& e)
{
    return insert(index, e.m_str);
}

//去掉字符串两端的空白字符（空格）
String& String::trim()
{
    int beg = 0;
    int end = m_length - 1;

    //确定中间部分字符串
    while (m_str[beg] == ' ') beg++;
    while (m_str[end] == ' ') end--;

    if(beg == 0)//当前面没空格的情况
    {
        m_str[end + 1] = '\0';
        m_length = end + 1;
    }
    else
    {
        for(int i=0,j=beg; j<=end; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[end - beg + 1] = '\0';
        m_length = end - beg + 1;
    }

    return *this;
}

int String::index_of(const char* s) const
{
    return kmp(m_str, s ? s : "");
}

int String::index_of(const String& e) const
{
    return kmp(m_str, e.m_str);
}

String& String::remove(int index, int len)
{
    if((0 <= index) && (index < m_length))
    {
        int n = index;
        int m = index + len;

        while((n < m) && (m < m_length))
        {
            m_str[n++] = m_str[m++];
        }

        m_str[n] = '\0';
        m_length = n;
    }
    return *this;
}

String& String::remove(const char* s)
{
    return remove(index_of(s), s ? strlen(s) : 0);
}

String& String::remove(const String& e)
{
    return remove(index_of(e), e.length());
}

String& String::replace(const char* t, const char* s)
{
    int index = index_of(t);

    if(index >= 0)
    {
        remove(t);
        insert(index, s);
    }

    return *this;
}

String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s);
}

String& String::replace(const char* t, const String& s)
{
    return replace(t, s.m_str);
}

String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}

String String::sub(int index, int len) const
{
    String ret;

    if((0 <= index) && (index < m_length))
    {
        if(len < 0) len = 0;
        if((len+index) > m_length) len = m_length - index;
        char * str = reinterpret_cast<char*>(malloc(len + 1));

        strncpy(str, m_str + index, len);
        str[len] = '\0';

        ret = str;
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter index Is Invalid ...");
    }

    return ret;
}

String::~String()
{
    free(m_str);
}
}
