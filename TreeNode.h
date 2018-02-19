#ifndef TREENODE_H
#define TREENODE_H
#include "Object.h"

namespace HsTL
{

template <class T>
class TreeNode : public Object
{
protected:
    bool m_flag;                   //用于判断节点是否在堆中申请，便于释放
    void* operator new(unsigned int size) throw()
    {
        return Object::operator new(size);
    }

    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);


public:
    T value;
    TreeNode<T>* parent;

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template <class T>
TreeNode<T>::~TreeNode()
{

}

}


#endif // TREENODE_H
