#ifndef SMARTPOINT_H
#define SMARTPOINT_H
#include "Pointer.h"
/*************************************
 * 18-1-31 17:10
**************************************/
namespace HsTL
{
template <class T>
class SmartPointer : public Pointer<T>
//两个指针不能指向同一片空间
{
public:
    SmartPointer(T* p=NULL) : Pointer<T>(p) {}

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer=obj.m_pointer;
        const_cast<SmartPointer<T>&>(obj).m_pointer=NULL;
    }

    SmartPointer<T>& operator=(const SmartPointer<T>& obj)
    {
        if(this!=&obj)
        {
            //异常安全
            T* tmp = this->m_pointer;
            this->m_pointer=obj.m_pointer;
            const_cast<SmartPointer<T>&>(obj).m_pointer=NULL;
            delete tmp;
        }
        return *this;
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
};
}
#endif // SMARTPOINT_H
