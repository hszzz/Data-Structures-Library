#ifndef BTREENODE_H
#define BTREENODE_H
#include "TreeNode.h"

namespace HsTL
{

//二叉树孩子节点的位置
enum BTNodePos
{
    ANY,
    LEFT,
    RIGHT
};
template <class T>
class BTreeNode : public TreeNode<T>
{
public:
    BTreeNode<T>* left;
    BTreeNode<T>* right;

    BTreeNode()
    {
        left = NULL;
        right = NULL;
    }

    static BTreeNode<T>* NewNode()
    {
        BTreeNode<T>* ret = new BTreeNode<T>();

        if(ret != NULL)
        {
            ret->m_flag = true;
        }

        return ret;
    }

};

}
#endif // BTREENODE_H
