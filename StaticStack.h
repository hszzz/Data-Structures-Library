#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "Exception.h"
/*****************************************
 * 原生数组模拟栈
 * 缺点：构造时需要重复调用元素类型的构造函数
 * 18-2-2 23:30
******************************************/

namespace HsTL
{
template <class T, int N>
class StaticStack : public Stack<T>
{
public:
    StaticStack()
    {
        m_top = -1;
        m_size = 0;
    }

    int capacity() const
    {
        return N;
    }

    void push(const T& e)
    {
        if(m_size < N)
        {
            m_space[m_top+1] = e; //先赋值，再改变栈的状态，异常安全
            m_top++;
            m_size++;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Space To push in Stack ...");
        }
    }

    void pop()
    {
        if(m_size > 0)
        {
            m_top--;
            m_size--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current Stack ...");
        }
    }

    T top() const
    {
        if(m_size > 0)
        {
            return m_space[m_top];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current Stack ...");
        }
    }

    void clear()
    {
        m_top = -1;
        m_size = 0;
    }

    int size() const
    {
        return m_size;
    }
protected:
    T m_space[N];
    int m_top;
    int m_size;
};
}

#endif // STATICSTACK_H
