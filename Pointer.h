#ifndef POINTER_H
#define POINTER_H
#include "Object.h"
/*********************************************
 * 1> 只能用来指向对空间中的单个变量
 * 2> 不同类型的智能指针对象不能混合使用
 * 3> 不要使用delete释放智能指针指向的堆空间
 * *******************************************
 *  18-1-31 19:09
 *********************************************/
namespace HsTL
{
template <class T>
class Pointer : public Object
//智能指针抽象类
{
public:
    Pointer(T* p = NULL)
    {
        m_pointer = p;
    }

    T* operator-> ()
    {
        return m_pointer;
    }

    T& operator* ()
    {
        return  *m_pointer;
    }

    const T* operator-> () const
    {
        return m_pointer;
    }

    const T& operator* () const
    {
        return  *m_pointer;
    }

    bool isNull() const
    {
        return (m_pointer == NULL);
    }

    T* get() const
    {
        return m_pointer;
    }

protected:
    T* m_pointer;
};
}
#endif // POINTER_H
