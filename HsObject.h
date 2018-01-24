#ifndef HSOBJECT_H
#define HSOBJECT_H

namespace HsTL
{
//顶层抽象父类，用来规范new delete运算符，所有子类都可以用它来动态类型识别
class HsObject
{
public:
    void* operator new (unsigned int size) throw();
    void operator delete (void* p);
    void* operator new[] (unsigned int size) throw();
    void operator delete[] (void* p);
    virtual ~HsObject() = 0;
};
}

#endif // HSOBJECT_H
