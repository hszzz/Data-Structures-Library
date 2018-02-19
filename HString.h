#ifndef HSTRING_H
#define HSTRING_H
#include "Object.h"
/************************************************
 * 字符串
 * 18-2-4 17:30
*************************************************/

namespace HsTL
{
class String
{
public:
    String();
    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* str() const;//与传统字符串进行互转操作

    bool operator == (const String& e) const;
    bool operator == (const char* s) const;

    bool operator != (const String& e) const;
    bool operator != (const char* s) const;

    bool operator  > (const String& e) const;
    bool operator  > (const char* s) const;

    bool operator  < (const String& e) const;
    bool operator  < (const char* s) const;

    bool operator  >= (const String& e) const;
    bool operator  >= (const char* s) const;

    bool operator  <= (const String& e) const;
    bool operator  <= (const char* s) const;

    String operator + (const String& e) const;
    String operator + (const char* s) const;
    String& operator += (const String& e);
    String& operator += (const char* s);

    //被减的字符串本身不应该被影响
    String operator - (const String& e) const;
    String operator - (const char* s) const;
    //-=，字符串本身要改变
    String& operator -= (const String& e);
    String& operator -= (const char* s);

    String& operator = (const String& e);
    String& operator = (const char* s);
    String& operator = (char c);

    char& operator [] (int index);
    char operator [] (int index) const;

    //判断字符串是否以指定的字符串开始或结束
    bool start_with(const char* s) const;
    bool start_with(const String& e) const;
    bool end_of(const char* s) const;
    bool end_of(const String& e) const;

    //在指定位置插入一个字符串
    String& insert(int index, const char* s);
    String& insert(int index, const String& e);

    //去掉字符串两端的空白字符（空格）,将空白字符放到最后面
    String& trim();

    //删除指定下标、指定长度的子串
    String& remove(int index, int len);

    //子串查找  返回位置下标
    int index_of(const char* s) const;
    int index_of(const String& e) const;

    //在字符串中删除指定的子串
    String& remove(const char* s);
    String& remove(const String& e);

    //子串替换
    String& replace(const char* t, const char* s);
    String& replace(const String& t, const char* s);
    String& replace(const char* t, const String& s);
    String& replace(const String& t, const String& s);

    //从字符串中在index位置，创建长度为len的子串
    String sub(int index, int len) const;

    ~String();
protected:
    char* m_str;
    int m_length;

    void init(const char* s);
    //判断字符串在指定比对长度下是否相等
    bool equal(const char* l, const char* r, int len) const;

    //KMP算法
    static int* make_pmt(const char* s); //获取字符串的部分匹配表
    static int kmp(const char* s/*被查找*/, const char* p/*需要查找*/); //返回子串开始时的位置
};

}

#endif // HSTRING_H
