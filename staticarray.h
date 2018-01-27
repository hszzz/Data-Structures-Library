#ifndef STATICARRAY_H
#define STATICARRAY_H
#include "Array.h"
namespace HsTL
{
template <class T, int N>
class StaticArray : public Array<T>
{
public:
    StaticArray()
    {
        this->m_array = m_space;
    }

    StaticArray(const StaticArray<T, N>& obj)
    {
        this->m_array = m_space;

        for(int i=0; i<N; i++)
        {
            m_space[i] = obj.m_space[i];
        }
    }

    StaticArray<T, N>& operator= (const StaticArray<T, N>& obj)
    {
        if(this != &obj)
        {
            for(int i=0; i<N; i++)
            {
                m_space[i] = obj.m_space[i];
            }
        }
    }

    int length() const
    {
        return N;
    }

protected:
    T m_space[N];
};
}
#endif // STATICARRAY_H
