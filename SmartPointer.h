#ifndef SMARTPOINT_H
#define SMARTPOINT_H
#include "HsObject.h"
namespace HsTL
{
template <class T>
class SmartPointer : public HsObject
{
public:
    SmartPointer(T* p=NULL)
    {
        m_pointer = p;
    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer=obj.m_pointer;
        const_cast<SmartPointer<T>&>(obj).m_pointer=NULL;
    }

    T& operator=(const SmartPointer<T>& obj)
    {
        if(this!=&obj)
        {
            delete this->m_pointer;
            this->m_pointer=obj.m_pointer;
            const_cast<SmartPointer<T>&>(obj).m_pointer=NULL;
        }
        return *this;
    }

    T* operator-> ()
    {
        return m_pointer;
    }

    T& operator* ()
    {
        return *m_pointer;
    }

    T* get()
    {
        return m_pointer;
    }

    bool isNull()
    {
        return (m_pointer==NULL);
    }

    ~SmartPointer()
    {
        if(m_pointer!=NULL)
        {
            delete m_pointer;
        }
    }

protected:
    T* m_pointer;
};
}
#endif // SMARTPOINT_H
