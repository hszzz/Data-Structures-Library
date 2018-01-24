#ifndef STATICLIST_H
#define STATICLIST_H
#include "SeqList.h"
namespace HsTL
{
template <class T, int N>
class StacticList : public SeqList<T>
{
public:
    StacticList()
    {
        this->m_array = m_space;
        this->m_length = 0;
    }

    int capacity() const
    {
        return N;
    }

protected:
    T m_space[N];
};
}

#endif // STATICLIST_H
