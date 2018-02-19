#ifndef BTREE_H
#define BTREE_H
#include "Tree.h"
#include "BTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

namespace HsTL
{

enum BTTraversal { PreOrder, InOrder, PostOrder, LevelOrder }; //前序、中序、后序、层次

template <class T>
class BTree : public Tree<T>
{
public:
    virtual bool insert(TreeNode<T> *node, BTNodePos pos)
    {
        bool ret = true;

        if(node != NULL)
        {
            if(this->m_root == NULL)
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                BTreeNode<T>* np = find(node->parent);

                if(np != NULL)
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(node), np, pos);
                }
                else
                {
                    THROW_EXCEPTION(InvalidParameterException, "Invalid Parent Tree Node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter Node Cannot Be NULL ... ");
        }

        return ret;
    }

    bool insert(TreeNode<T>* node)
    {
        return insert(node, ANY);
    }

    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
    {

        bool ret = true;

        BTreeNode<T>* node = BTreeNode<T>::NewNode();

        if(node == NULL)
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To Create New Node ...");
        }
        else
        {
            node->value = value;
            node->parent = parent;

            ret = insert(node, pos);

            if(!ret)
            {
                delete node;
            }
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        return insert(value, parent, ANY);
    }

    SharedPointer< Tree<T> > remove(const T& value)
    {
        BTree<T>* ret = NULL;

        BTreeNode<T>* node = find(value);

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "Cannot Find The Tree Node Via Value ...");
        }
        else
        {
            remove(node, ret);
            m_queue.clear();
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        BTree<T>* ret = NULL;

        node = find(node);          //查看node是否在树中

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter Node Is Invalid ...");
        }
        else
        {
            remove(dynamic_cast<BTreeNode<T>*>(node), ret);
            m_queue.clear();
        }

        return ret;
    }

    BTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }

    BTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<BTreeNode<T>*>(node));
    }

    BTreeNode<T>* root() const
    {
        return dynamic_cast<BTreeNode<T>*>(this->m_root);
    }

    int degree() const
    {
        return degree(root());
    }

    int count() const
    {
        return count(root());
    }

    int height() const
    {
        return height(root());
    }

    void clear()
    {
        free(root());

        m_queue.clear();

        this->m_root = NULL;
    }

    ~BTree()
    {
        clear();
    }

    //层次遍历
    bool begin()
    {
        bool ret = (root() != NULL);

        if(ret)
        {
            m_queue.clear();
            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if(ret)
        {
            BTreeNode<T>* node = m_queue.front();

            m_queue.remove();

            if(node->left != NULL)
            {
                m_queue.add(node->left);
            }
            if(node->right != NULL)
            {
                m_queue.add(node->right);
            }

        }

        return ret;
    }

    T current()
    {
        if(!end())
        {
            return m_queue.front()->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No Value At current Position ...");
        }
    }

    //顺序遍历
    SharedPointer<Array<T>> traversal(BTTraversal order)
    {
        DynamicArray<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);

        ret = new DynamicArray<T>(queue.length());
        if(ret != NULL)
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front()->value);
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To Create return Array ...");
        }

        return ret;
    }

    //将当前二叉树复制一份
    SharedPointer<BTree<T>> clone() const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret != NULL)
        {
            ret->m_root = clone(root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New tree ...");
        }

        return ret;
    }

    //二叉树的比较操作
    bool operator == (const BTree<T>& btree) const
    {
        return equal(root(), btree.root());
    }

    bool operator != (const BTree<T>& btree) const
    {
        return !(*this == btree);
    }

    //二叉树相加（对应节点元素相加），以智能指针方式返回
    SharedPointer<BTree<T>> add(const BTree<T>& btree) const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret != NULL)
        {
            ret->m_root = add(root(), btree.root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New Tree ...");
        }

        return ret;
    }

    //线索化
    BTreeNode<T>* thread(BTTraversal order)
    {
        BTreeNode<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);

        ret = connect(queue);

        this->m_root = NULL;

        m_queue.clear();

        return ret;
    }

protected:
    LinkQueue<BTreeNode<T>*> m_queue;

    //以node为根节点，查找元素value
    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value) const
    {
        BTreeNode<T>* ret =NULL;

        if(node != NULL)
        {
            if(node->value == value)
            {
                ret = node;
            }
            else
            {
                if(ret == NULL)
                {
                    ret = find(node->left, value);
                }

                if(ret == NULL)
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    //以node为根节点，查找节点obj
    virtual BTreeNode<T>* find(BTreeNode<T> *node, BTreeNode<T>* obj) const
    {
        BTreeNode<T>* ret = NULL;

        if(node == obj)
        {
            ret = node;
        }
        else
        {
            if(node != NULL)
            {
                if(ret == NULL)
                {
                    ret = find(node->left, obj);
                }

                if(ret == NULL)
                {
                    ret = find(node->right, obj);
                }
            }
        }

        return ret;
    }

    //插入新节点n，在父节点np的pos位置
    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;

        if(pos == ANY)
        {
            if(np->left == NULL)
            {
                np->left = n;
            }
            else if(np->right == NULL)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }

        }
        else if(pos == LEFT)
        {
            if(np->left == NULL)
            {
                np->left = n;
            }
            else
            {
                ret = false;
            }
        }
        else if(pos == RIGHT)
        {
            if(np->right == NULL)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    //删除以node为根节点的子树，并用ret把该子树间接返回
    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();

        if(ret == NULL)
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New Tree ...");
        }
        else
        {
            if(root() == node)
            {
                this->m_root = NULL;
            }
            else
            {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);

                if(parent->left == node)
                {
                    parent->left = NULL;
                }
                else if(parent->right == node)
                {
                    parent->right = NULL;
                }

                node->parent = NULL;
            }
            ret->m_root = node;
        }
    }

    //递归free
    virtual void free(BTreeNode<T>* node)
    {
        if(node != NULL)
        {
            free(node->left);
            free(node->right);

            if(node->flag())
            {
                delete node;
            }
        }
    }

    //递归count
    int count(BTreeNode<T>* node) const
    {
        /*
        int ret = 0;

        if(node != NULL)
        {
            ret = count(node->left) + count(node->right) + 1;
        }
        */
        return (node != NULL) ? (count(node->left) + count(node->right) + 1) : 0 ;
    }

    //递规height
    int height(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            int lh = height(node->left);
            int rh = height(node->right);
            ret = ((lh > rh) ? lh : rh) + 1;
        }


        return ret;
    }

    //递归degree
    int degree(BTreeNode<T>* node) const
    {
        int ret = 0;

        ret = (!!node->left + !!node->right);

        if(ret < 2)
        {
            int dl = degree(node->left);
            if(ret < dl)
            {
                ret = dl;
            }
        }
        if(ret < 2)
        {
            int dr = degree(node->right);
            if(ret < dr)
            {
                ret = dr;
            }
        }

        return ret;
    }

    //递归clone
    BTreeNode<T>* clone(BTreeNode<T>* node) const
    {
        BTreeNode<T>* ret = NULL;

        if(node != NULL)
        {
            ret = BTreeNode<T>::NewNode();

            if(ret != NULL)
            {
                ret->value = node->value;
                ret->left = clone(node->left);
                ret->right = clone(node->right);

                if(ret->left != NULL)
                {
                    ret->left->parent = ret;
                }

                if(ret->right != NULL)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New Node ...");
            }
        }

        return ret;
    }

    //先序遍历
    void preOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != NULL)
        {
            queue.add(node);
            preOrderTraversal(node->left, queue);
            preOrderTraversal(node->right, queue);
        }
    }

    //中序遍历
    void inOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != NULL)
        {
            inOrderTraversal(node->left, queue);
            queue.add(node);
            inOrderTraversal(node->right, queue);
        }
    }

    //后序遍历
    void postOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != NULL)
        {

            postOrderTraversal(node->left, queue);
            postOrderTraversal(node->right, queue);
            queue.add(node);
        }
    }

    //可保存节点间的关系到queue的层次遍历，便于实现线索化
    void levelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != NULL)
        {
            LinkQueue<BTreeNode<T>*> tmp;
            tmp.add(node);

            while(tmp.length() > 0)
            {
                BTreeNode<T>* n = tmp.front();

                if(n->left != NULL)
                {
                    tmp.add(n->left);
                }
                if(n->right != NULL)
                {
                    tmp.add(n->right);
                }

                tmp.remove();
                queue.add(n);
            }
        }
    }

    //递归判断两个二叉树是否相等
    bool equal(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        if(lh == rh)
        {
            return true;
        }
        else if((lh != NULL) && (rh != NULL))
        {
            return (lh->value == rh->value) && equal(lh->left, rh->left) && equal(lh->right, rh->right);
        }
        else
        {
            return false;
        }
    }

    //递归元素相加
    BTreeNode<T>* add(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        BTreeNode<T>* ret = NULL;

        if((lh == NULL) && (rh != NULL))
        {
            ret = clone(rh);
        }
        else if((lh != NULL) && (rh == NULL))
        {
            ret = clone(lh);
        }
        else if((lh != NULL) && (rh != NULL))
        {
            ret = BTreeNode<T>::NewNode();

            if(ret != NULL)
            {
                ret->value = lh->value + rh->value;
                ret->left = add(lh->left, rh->left);
                ret->right = add(lh->right, rh->right);

                if(ret->left != NULL)
                {
                    ret->left->parent = ret;
                }
                if(ret->right != NULL)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New Node ...");
            }
        }

        return ret;
    }

    //选择顺序遍历二叉树，并把节点放入队列中
    void traversal(BTTraversal order, LinkQueue<BTreeNode<T>*>& queue)
    {
        switch (order)
        {
        case PreOrder:
            preOrderTraversal(root(), queue);
            break;

        case InOrder:
            inOrderTraversal(root(), queue);
            break;

        case PostOrder:
            postOrderTraversal(root(), queue);
            break;

        case LevelOrder:
            levelOrderTraversal(root(), queue);
            break;

        default:
            THROW_EXCEPTION(InvalidParameterException, "Parameter Order Is Invalid ...");
            break;
        }
    }

    //线索化二叉树，将遍历得到的queue中元素连接成双向链表,返回双向链表首节点地址
    //原来的二叉树将不复存在
    BTreeNode<T>* connect(LinkQueue<BTreeNode<T>*>& queue)
    {
        BTreeNode<T>* ret = NULL;

        if(queue.length() > 0)
        {
            ret = queue.front();

            //把第一个元素取出来
            BTreeNode<T>* slider = queue.front();

            queue.remove();

            slider->left = NULL;

            while (queue.length() > 0)
            {
                slider->right = queue.front();
                queue.front()->left = slider;

                slider = queue.front();
                queue.remove();
            }

            slider->right = NULL;
        }

        return ret;
    }
};

}
#endif // BTREE_H
