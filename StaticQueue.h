#ifndef STATICQUEUE_H
#define STATICQUEUE_H
#include "Queue.h"
#include "Exception.h"

/***************************************
 * 原生数组模拟队列，运用了循环记数法
 * 缺点：构造时需要重复调用元素类型的构造函数
 * 18-2-3 23:00
****************************************/

namespace HsTL
{

template <class T, int N>
class StaticQueue : public Queue<T>
{
public:
    StaticQueue()
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }

    int capacity() const
    {
        return N;
    }

    void add(const T& e)
    {
        if(m_length < N)
        {
            m_space[m_rear] = e;
            m_rear = (m_rear + 1) % N;//循环计数法
            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Space In Current queue ...");
        }
    }

    void remove()
    {
        if(m_length > 0)
        {
            m_front = (m_front + 1) % N;
            m_length--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current queue ...");
        }
    }

    T front() const
    {
        if(m_length > 0)
        {
            return m_space[m_front];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current queue ...");
        }
    }


    void clear()
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }

    int length() const
    {
        return m_length;
    }

protected:
    T m_space[N];
    int m_front;
    int m_rear;
    int m_length;
};

}

#endif // STATICQUEUE_H
