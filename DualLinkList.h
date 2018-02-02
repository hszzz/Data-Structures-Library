#ifndef DUALLINKLIST_H
#define DUALLINKLIST_H
#include "List.h"
#include "Exception.h"

namespace HsTL
{
template <class T>
class DualLinkList : public List<T>
{
public:
    DualLinkList()
    {
        m_header.next = NULL;
        m_header.pre = NULL;
        m_length = 0;
        m_step = 1;
        m_current = NULL;
    }

    bool insert(const T &e)
    {
        return insert(m_length, e);
    }

    bool insert(int index, const T& e)
    {
        bool ret = ((0 <= index) && (index <= m_length));

        if(ret)
        {
            Node* node = create();

            if(node != NULL)
            {
                Node* current = position(index);
                Node* next = current->next;

                node->value = e;

                node->next = next;
                current->next = node;

                if(current != reinterpret_cast<Node*>(&m_header))
                {
                    node->pre = current;
                }
                else
                {
                    node->pre = NULL;
                }

                if(next != NULL)
                {
                    next->pre = node;
                }

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To insert New Node ...");
            }
        }
        return ret;
    }

    bool remove(int index)
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            Node* current = position(index);
            Node* toDel = current->next;
            Node* next = toDel->next;

            if(m_current == toDel)
            {
                m_current = toDel->next;
            }

            current->next = toDel->next;

            if(next != NULL)
            {
                next->pre = toDel->pre;
            }

            m_length--;

            destory(toDel);
        }

        return ret;
    }

    bool set(int index, const T& e)
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            position(index)->next->value = e;
        }

        return ret;
    }

    virtual T get(int index) const
    {
        T ret;

        if(get(index, ret))
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid Parameter index To Get Element ...");
        }

        return ret;
    }

    bool get(int index, T& e) const
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            e = position(index)->next->value;
        }

        return ret;
    }

    int find(const T& e) const
    {
        int ret = -1;
        int i = 0;
        Node* node = m_header.next;
        while(node)
        {
            if(node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                node = node->next;
                i++;
            }
        }
        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        while(m_length > 0)
        {
            remove(0);
        }
    }

    //以下成员函数为了实现链表遍历的时间复杂度为O(n)
    /* 具体示例...
    for(list.move(0); !list.end(); list.next())
    {
        cout << list.current() << endl;
    }
    */

    //move用于将游标定义到目标位置
    virtual bool move(int i/*起始位置*/, int step = 1/*每次移动的步数*/)
    {
        bool ret = (0 <= i) && (i < m_length) && (step > 0);

        if(ret)
        {
            m_current = position(i)->next;
            m_step = step;
        }

        return ret;
    }
    //end用于判断当前遍历是否结束
    virtual bool end()
    {
        return (m_current == NULL);
    }

    //current用于返回当前游标指向的节点的值
    virtual T current()
    {
        if(!end())
        {
            return m_current->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Value At current Position ...");
        }
    }

    //next用于向后移动游标，移动m_step步
    virtual bool next()
    {
        int i = 0;
        while((i < m_step) && !end())
        {
            m_current = m_current->next;
            i++;
        }
        return (i == m_step);
    }

    //pre用于向前移动游标，移动m_step步
    virtual bool pre()
    {
        int i = 0;
        while((i < m_step) && !end())
        {
            m_current = m_current->pre;
            i++;
        }
        return (i == m_step);
    }

    ~DualLinkList()
    {
        clear();
    }

protected:
    struct Node : public Object
    {
        T value;
        Node* next;
        Node* pre;
    };

    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
        Node* pre;
    } m_header;

    int m_length;

    int m_step;
    Node* m_current;

    //求出要操作的节点的位置
    Node* position(int index) const
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);

        for(int p=0; p<index; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destory(Node* pn)
    {
        delete pn;
    }


};
}

#endif // DUALLINKLIST_H
