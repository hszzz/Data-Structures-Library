#ifndef SEQLIST_H
#define SEQLIST_H
#include "HsObject.h"
#include "Exception.h"
namespace HsTL
{
template <class T>
class SeqList : public HsObject
{
public:
    bool insert(int index, const T& e)
    {
        //判断插入位置是否合理
        bool ret = ((0 <= index) && (index <= m_length));
        ret = ret && (m_length < capacity());

        if(ret)
        {
            for(int i=m_length; i>=index; i--)
            {
                m_array[i+1] = m_array[i];
            }
            m_array[index] = e;
            ++m_length;
        }

        return ret;
    }

    bool remove(int index)
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            for(int i=index; i<m_length-1; i++)
            {
                m_array[i] = m_array[i+1];
            }
            --m_length;
        }

        return ret;
    }

    bool set(int index, const T& e)
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            m_array[index] = e;
        }

        return ret;
    }

    bool get(int index,T& e) const
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            e = m_array[index];
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        m_length = 0;
    }

    //非const List版本
    T& operator[] (int index)
    {
        if((0 <= index) && (index < m_length))
        {
            return m_array[index];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException,"Parameter index is invalid...");
        }
    }

    //const List版本
    T operator [](int index) const
    {
        return (const_cast<SeqList<T>&>(*this))[index];
    }

    //容量大小，由子类实现
    virtual int capacity() const = 0;

protected:
    T* m_array;
    int m_length;
};

}
#endif // SEQLIST_H
