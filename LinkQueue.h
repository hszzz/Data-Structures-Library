#ifndef LINKQUEUE_H
#define LINKQUEUE_H
#include "Queue.h"
#include "LinuxList.h"
#include "Exception.h"
/**********************************
 * 用LinuxList实现队列，效率高
 * 18-2-3 23:30
***********************************/
namespace HsTL
{

template <class T>
class LinkQueue : public Queue<T>
{
public:
    LinkQueue()
    {
        m_length = 0;
        INIT_LIST_HEAD(&m_header);
    }

    void add(const T& e)
    {
        Node* node = new Node();

        if(node != NULL)
        {
            node->value = e;

            list_add_tail(&node->head, &m_header);

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Memory To add new Element ...");
        }
    }

    void remove()
    {
        if(m_length > 0)
        {
            list_head* toDel = m_header.next;

            list_del(toDel);

            m_length--;

            delete list_entry(toDel, Node, head);
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
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Element In Current queue ...");
        }
    }

    void clear()
    {
        while (m_length > 0)
        {
            remove();
        }
    }

    int length() const
    {
        return m_length;
    }

    ~LinkQueue()
    {
        clear();
    }

protected:
    struct Node : public Object
    {
        list_head head;
        T value;
    };

    list_head m_header;
    int m_length;
};

}


#endif // LINKQUEUE_H
