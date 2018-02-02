#ifndef CIRCLE_H
#define CIRCLE_H
#include "linklist.h"

namespace HsTL
{
template <class T>
class CircleList : public LinkList<T>
{
public:
    bool insert(const T& e)
    {
        return insert(this->m_length, e);
    }

    bool insert(int index, const T& e)
    {
        bool ret = true;

        //找到合理的位置
        index = index % (this->m_length + 1);

        ret = LinkList<T>::insert(index, e);

        //当插入节点为0号时，则需要首位相连
        if(ret && (index==0))
        {
            last_to_first();
        }

        return ret;
    }

    bool remove(int index)
    {
        bool ret = true;

        index = mod(index);

        if(index == 0)
        {
            Node* toDel = this->m_header.next;

            if(toDel != NULL)
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if(this->m_length > 0)
                {
                    last_to_first();

                    if(this->m_current == toDel)
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }
                this->destroy(toDel);
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(index);
        }

        return ret;
    }

    bool set(int index, const T& e)
    {
        return LinkList<T>::set(mod(index), e);
    }

    T get(int index) const
    {
        return LinkList<T>::get(mod(index));
    }

    bool get(int index, const T& e) const
    {
        return LinkList<T>::get(mod(index), e);
    }

    int find(const T& e) const
    {
        int ret = -1;

        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i++)
        {
            if(slider->value == e)
            {
                ret = i;
                break;
            }
            slider = slider->next;
        }
        return ret;
    }

    void clear()
    {
        while(this->m_length > 1)
        {
            //移除1号节点，而非0号，可以提高性能
            remove(1);
        }
        if(this->m_length == 1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    //以下为遍历操作服务
    bool move(int index, int step)
    {
        return LinkList<T>::move(mod(index), step);
    }

    bool end()
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }

    ~CircleList()
    {
        clear();
    }

protected:
    typedef typename LinkList<T>::Node Node;
    //找到最后一个节点
    Node* last() const
    {
        return this->position(this->m_length-1)->next;
    }

    //最后一个节点连上第一个节点，形成循环
    void last_to_first() const
    {
        last()->next = this->m_header.next;
    }

    //如果插入位置过大，则可以通过mod找到合理位置，避免性能浪费
    int mod(int index) const
    {
        return (this->m_length == 0) ? 0 : (index % this->m_length);
    }
};
}
#endif // CIRCLE_H
