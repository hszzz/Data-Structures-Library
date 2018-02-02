#ifndef ARRAY_H
#define ARRAY_H
#include "Object.h"
#include "Exception.h"
namespace HsTL
{
template <class T>
class Array : public Object
{
public:
    virtual bool set(int index, const T& e)
    {
        bool ret = ((0 <= index) && (index < length()));

        if(ret)
        {
            m_array[index] = e;
        }

        return ret;
    }

    virtual bool get(int index, T& e) const
    {
        bool ret = ((0 <= index) && (index < length()));

        if(ret)
        {
            e = m_array[index];
        }

        return ret;
    }

    T& operator[] (int index)
    {
        if((0 <= index) && (index < length()))
        {
            return m_array[index];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter index Is Invalid ...");
        }
    }

    T operator[] (int index) const
    {
        return (const_cast<Array<T>&>(*this))[index];
    }

    virtual int length() const = 0;
protected:
    T* m_array;
};
}
#endif // ARRAY_H
