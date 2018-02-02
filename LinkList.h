#ifndef LINKLIST_H
#define LINKLIST_H
#include "Exception.h"
#include "List.h"
/**********************************************
 *对于内置基础类型，顺序表和单链表的效率不相上下
 * 对于自定义类型，顺序表在效率上低于单链表
***********************************************/

/**********************************************
 * 对于单链表来说
 * 频繁的插入、删除节点，会使内存存在很多内存碎片
 * 会导致系统运行速度变慢
***********************************************/
namespace HsTL
{
template <class T>
class LinkList : public List<T>
{
public:
    LinkList()
    {
        m_header.next = NULL;
        m_length = 0;
        m_step = 1;
        m_current = NULL;
    }

    bool insert(const T& e)
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
                /*
                Node* current = &m_header;

                for(int i=0; i<index; i++)
                {
                    current = current->next;
                }
                */
                node->value = e;
                node->next = current->next;
                current->next = node;

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New Node ...");
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
            /*
            Node* current = &m_header;

            for(int i=0; i<index; i++)
            {
                current = current->next;
            }
            */
            Node* toDel = current->next;
            if(m_current == toDel)
            {
                m_current = toDel->next;
            }

            current->next = toDel->next;

            m_length--;

            destroy(toDel);
        }

        return ret;
    }

    bool set(int index, const T& e)
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            Node* current = position(index);
            /*
            Node* current = &m_header;

            for(int i=0; i<index; i++)
            {
                current = current->next;
            }
            */
            current->next->value = e;
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
    }

    bool get(int index, T& e) const
    {
        bool ret = ((0 <= index) && (index < m_length));

        if(ret)
        {
            Node* current = position(index);
            /*
            Node* current = &m_header;

            for(int i=0; i<index; i++)
            {
                current = current->next;
            }
            */
            e = current->next->value;
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        while (m_header.next)
        {
            Node* toDel = m_header.next;
            m_header.next = toDel->next;
            m_length--;
            destroy(toDel);
        }
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

    //next用于移动游标
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

    ~LinkList()
    {
        clear();
    }

protected:
    struct Node : public Object
    {
        T value;
        Node* next;
    };

    mutable struct : public Object //!!!
    {
        char reserved[sizeof(T)];
                  Node* next;} m_header;
    int m_length;

    //遍历链表使用
    Node* m_current;//游标
    int m_step;//记录游标移动节点的数目

    //代码优化，定位函数
    Node* position(int index) const
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);

        for(int i=0; i<index; i++)
        {
            ret = ret->next;
        }

        return ret;
    }

    //进一步封装，封装Node的创建与删除，增强扩展性
    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* pn)
    {
        delete pn;
    }
};
}

#endif // LINKLIST_H
