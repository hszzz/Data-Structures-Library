#ifndef GTREENODE_H
#define GTREENODE_H
#include "TreeNode.h"
#include "LinkList.h"


namespace HsTL
{
template <class T>
class GTreeNode : public TreeNode<T>
{
public:
    LinkList<GTreeNode<T>*> child; //用于保存子节点地址

    static GTreeNode<T>* NewNode()
    {
        GTreeNode<T>* ret = new GTreeNode<T>();

        if(ret != NULL)
        {
            ret->m_flag = true;
        }

        return ret;
    }

};

}
#endif // GTREENODE_H
