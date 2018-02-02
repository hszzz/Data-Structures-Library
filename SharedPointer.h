#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H
#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"
/*********************************************
 * 计数机制，使多个智能指针合法的指向同一片堆空间
 * *******************************************
 * 18-1-31 18:39
**********************************************/
namespace HsTL
{
template <class T>
class SharedPointer : public Pointer<T>
//多个指针可以指向同一个空间，并且可以智能释放
//最大程度模拟原生指针
{
public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if(p)
        {
            this->m_ref = static_cast<int*>((std::malloc(sizeof(int))));

            if(this->m_ref)
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create SharedPointer ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
        /*
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if(this->m_ref)
        {
            (*this->m_ref)++;
        }
        */
    }

    SharedPointer<T>& operator= (const SharedPointer<T>& obj)
    {
        if(this != &obj)
        {
            clear();
            assign(obj);
            /*
            this->m_ref = obj.m_ref;
            this->m_pointer = obj.m_pointer;

            if(this->m_ref)
            {
                (*this->m_ref)++;
            }
            */

        }
        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if(ref)
        {
            (*ref)--;
            if(*ref == 0)
            {
                free(ref);
                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }

protected:
    int* m_ref;//计数，用来记录有多少指针指向同一片空间

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if(this->m_ref)
        {
            (*this->m_ref)++;
        }
    }
};

template <typename T>
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());
}
template <typename T>
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
}
}
#endif // SHAREDPOINTER_H
