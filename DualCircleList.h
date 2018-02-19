#ifndef DUALCIRCLELIST_H
#define DUALCIRCLELIST_H
#include "LinuxList.h"
#include "DualLinkList.h"
/********************************************
 * 对Linux内核链表进行二次封装，得到自己的双向循环链表
 * 18-2-2 10:30
*********************************************/
namespace HsTL
{
template <class T>
class DualCircleList : public DualLinkList<T>
{
public:
    DualCircleList()
    {
        this->m_length = 0;
        this->m_step = 1;
        m_current = NULL;
        INIT_LIST_HEAD(&m_header);
    }

    bool insert(const T& e)
    {
        return insert(this->m_length, e);
    }

    bool insert(int index, const T& e)
    {
        bool ret = true;
        Node* node = new Node();

        index = index % (this->m_length + 1);

        if(node != NULL)
        {
            node->value = e;
            list_add_tail(&node->head, position(index)->next);

            this->m_length++;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To insert New Element ...");
        }

        return ret;
    }

    bool remove(int index)
    {
        bool ret = true;

        index = mod(index);

        ret = ((0 <= index) && (index < this->m_length));

        if(ret)
        {
            list_head* toDel = position(index)->next;

            if(m_current == toDel)
            {
                m_current = toDel->next;
            }

            list_del(toDel);

            this->m_length--;
            delete list_entry(toDel,Node,head);
        }

        return ret;
    }

    bool set(int index, const T& e)
    {
        bool ret = true;

        index = mod(index);

        ret = ((0 <= index) && (index < this->m_length));

        if(ret)
        {
            list_entry(position(index)->next, Node, head)->value = e;
        }

        return ret;
    }

    T get(int index) const
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
        bool ret = true;
        index = mod(index);
        ret = ((0 <= index) && (index < this->m_length));

        if(ret)
        {
            e = list_entry(position(index)->next, Node, head)->value;
        }

        return ret;
    }

    int find(const T& e) const
    {
        int ret = -1;
        int i = 0;
        list_head* slider =NULL;

        list_for_each(slider, &m_header)
        {
            if(list_entry(slider, Node, head)->value == e)
            {
                ret = i;
                break;
            }
            i++;
        }

        return ret;
    }

    int length() const
    {
        return this->m_length;
    }

    void clear()
    {
        while(this->m_length > 0)
        {
            remove(0);
        }
    }

    bool move(int i, int step=1)
    {
        bool ret = (step > 0);

        i = mod(i);

        ret = ret && ((0 <= i) && (i < this->m_length));

        if(ret)
        {
            m_current = position(i)->next;

            this->m_step = step;
        }

        return ret;
    }

    virtual bool end()
    {
        return (m_current == NULL) || (this->m_length == 0);
    }

    virtual T current()
    {
        if(!end())
        {
            return list_entry(m_current, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Value At current Position ...");
        }
    }

    bool next()
    {
        int i = 0;
        while((i < this->m_step) && !end())
        {
           if(m_current != &m_header)
           {
               m_current = m_current->next;
               i++;
           }
           else
           {
               m_current = m_current->next;
           }
        }

        if(m_current == &m_header)
        {
            m_current = m_current->next;
        }

        return (i == this->m_step);
    }

    bool pre()
    {
        int i = 0;
        while((i < this->m_step) && !end())
        {
           if(m_current != &m_header)
           {
               m_current = m_current->prev;
               i++;
           }
           else
           {
               m_current = m_current->prev;
           }
        }

        if(m_current == &m_header)
        {
            m_current = m_current->prev;
        }
        return (i == this->m_step);
    }

    ~DualCircleList()
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
    list_head* m_current;

    list_head* position(int index) const
    {
        list_head* ret = const_cast<list_head*>(&m_header);

        for(int p=0; p<index; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    int mod(int index) const
    {
        return (this->m_length == 0) ? 0 : (index % this->m_length);
    }


};

}

#endif // DUALCIRCLELIST_H

/*示例程序
int main()
{
    DualCircleList<int> dcl;

    for(int i=0; i<5; i++)
    {
        dcl.insert(i);
        dcl.insert(5);
    }

    cout << "begin" << endl;

    dcl.move(dcl.length()-1);

    while(dcl.find(5) != -1)
    {
        if(dcl.current() == 5)
        {
            cout << dcl.current() << endl;

            dcl.remove(dcl.find(dcl.current()));
        }
        else
        {
            dcl.pre();
        }
    }

    cout << "end" << endl;

    int i = 0;

    for(dcl.move(dcl.length()-1); (i < dcl.length()) && !dcl.end(); dcl.pre(), i++)
    {
        cout << dcl.current() << endl;
    }
    return 0;
}
*/
