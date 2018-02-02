#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H
/***********************************
 * 用于解决LinkList易产生内存碎片的缺陷
 * 18-01-29 17:32
************************************/
#include "linklist.h"

namespace HsTL
{
template <class T, int N>
class StaticLinkList : public LinkList<T>
{
public:
    StaticLinkList()
    {
        for(int i=0; i<N; i++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }

protected:
    typedef typename LinkList<T>::Node Node;//1-> 应为Node与模板T挂钩，所以要用LinkList<T>::Node
                                            //2-> 编译器不知道Node到底是静态成员变量还是个类型名，加typename就代表类型

    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc) //重载new，使其申请的空间其实就是loc
        {
            (void)size; //防止未用size的报错
            return loc;
        }
    };

    unsigned char m_space[sizeof(SNode)*N];

    int m_used[N]; //用于标记此处空间是否已经被使用

    Node* create()
    {
        SNode* ret = NULL;

        for(int i=0; i<N; i++)
        {
            if(!m_used[i])
            {
                ret = reinterpret_cast<SNode*>(m_space) + i;  //找到真正空间的地址
                ret = new(ret) SNode();
                m_used[i] = 1;
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn);
        for(int i=0; i<N; i++)
        {
            if(pn == (space + i))
            {
                m_used[i] = 0;
                psn->~SNode();
                break;
            }
        }
    }
};

}
#endif // STATICLINKLIST_H
