#ifndef HSOBJECT_H
#define HSOBJECT_H
#include <cstddef>
namespace HsTL
{
//顶层抽象父类，用来规范new delete运算符，所有子类都可以用它来动态类型识别
class Object
{
public:
    void* operator new (unsigned int size) throw();
    void operator delete (void* p);
    void* operator new[] (unsigned int size) throw();
    void operator delete[] (void* p);
    //为了实现容器的find操作自定义类型不报错，自定义类型应该尽量继承顶层抽象父类Object
    bool operator ==(const Object& obj);
    bool operator !=(const Object& obj);
    virtual ~Object() = 0;
};
}

#endif // HSOBJECT_H
