#ifndef LINKSTACK_H
#define LINKSTACK_H
#include "Stack.h"
#include "LinkList.h"
/**********************************
 * 通过组合LinkList，用链表的操作来实现栈
 * 18-2-2 23:51
***********************************/
namespace HsTL
{
template <class T>
class LinkStack : public Stack<T>
{
public:
    void push(const T& e)
    {
        m_list.insert(0, e);
    }

    void pop()
    {
        if(m_list.length() > 0)
        {
            m_list.remove(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current Stack ...");
        }
    }

    T top() const
    {
        if(m_list.length() > 0)
        {
            return m_list.get(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current Stack ...");
        }
    }

    void clear()
    {
        m_list.clear();
    }

    int size() const
    {
        return m_list.length();
    }
protected:
    LinkList<T> m_list;
};
}
#endif // LINKSTACK_H
