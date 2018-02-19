#ifndef GTREE_H
#define GTREE_H
#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "linkqueue.h"

namespace HsTL
{

template <class T>
class GTree : public Tree<T>
{
public:
    GTree() { }

    bool insert(TreeNode<T>* node)
    {
        bool ret = true;

        if(node != NULL)
        {
            if(this->root() == NULL)
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                GTreeNode<T>* np = find(node->parent);

                if(np != NULL)
                {
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);

                    if(np->child.find(n) < 0)
                    {
                        np->child.insert(n);
                    }
                }
                else
                {
                    THROW_EXCEPTION(InvalidOperationException, "Invalid Parent Tree node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter Node cannot be NULL ...");
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;

        GTreeNode<T>* node = GTreeNode<T>::NewNode();

        if(node != NULL)
        {
            node->value = value;
            node->parent = parent;

            insert(node);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To insert New Tree Node ...");
        }

        return ret;
    }

    //返回要删除节点的子树的 智能指针，不用考虑内存如何释放的问题
    SharedPointer< Tree<T> > remove(const T& value)
    {
        GTree<T>* ret = NULL;
        GTreeNode<T>* node = find(value);       //查看value是否在树中

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "Cannot Find The Node Via Parameter Value ...");
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
        GTree<T>* ret = NULL;
        node = find(node);                  //查看node是否在树中

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter Node Is Invalid ...");
        }
        else
        {
            remove(dynamic_cast<GTreeNode<T>*>(node), ret);
            m_queue.clear();
        }

        return ret;
    }

    GTreeNode<T>* find(const T& value) const       //基于元素值的查找
    {

        return find(root(), value);
    }

    GTreeNode<T>* find(TreeNode<T>* node) const    //基于节点的查找
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    GTreeNode<T>* root() const
    {
        return dynamic_cast<GTreeNode<T>*>(this->m_root);
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
        this->m_root = NULL;

        m_queue.clear();
    }


    //层次遍历
    /*for(t.begin(); !t.end(); t.next())
    {
        cout << t.current() << endl;
    }*/

    //为遍历做初始化
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

    //判断当前的遍历是否完成
    bool end()
    {
        return (m_queue.length() == 0);
    }

    //将队头弹出，再把弹出节点的孩子压入队中
    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if(ret)
        {
            GTreeNode<T>* node = m_queue.front();

            m_queue.remove();

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                m_queue.add(node->child.current());
            }
        }

        return ret;
    }

    //返回当前游标所指向节点的元素
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

    ~GTree()
    {
        clear();
    }
protected:
    LinkQueue<GTreeNode<T>*> m_queue;

    //在根节点为node的树中找value，便于递归
    GTreeNode<T>* find(GTreeNode<T>* node, const T& value) const
    {
        GTreeNode<T>* ret = NULL;

        if(node != NULL)
        {
            if(node->value == value)
            {
                return node;
            }
            else
            {
                for(node->child.move(0); !node->child.end() && (ret == NULL); node->child.next())
                {
                    ret = find(node->child.current(), value);
                }
            }
        }

        return ret;
    }

    //在根节点为node的树中找节点obj
    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = NULL;

        if(node == obj)
        {
            return node;
        }
        else
        {
            if(node != NULL)
            {
                for(node->child.move(0); !node->child.end() && (ret == NULL); node->child.next())
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }

    //释放在堆中的节点
    void free(GTreeNode<T>* node)
    {
        if(node != NULL)
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                free(node->child.current());
            }

            if(node->flag())
            {
               delete node;
            }
        }
    }

    //删除以node为根节点的子树，并用ret把该子树间接返回
    void remove(GTreeNode<T>* node, GTree<T>*& ret)
    {
        ret = new GTree<T>();

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
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(node->parent)->child;
                child.remove(child.find(node));

                node->parent = NULL;
            }

            ret->m_root = node;
        }

    }

    //求以node作为根节点的树的节点数
    int count(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            ret = 1;
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
            }
        }

        return ret;
    }

    //用来获取以node作为根节点的树的高度
    int height(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int h = height(node->child.current());

                //找最大高度
                if(ret < h)
                {
                    ret = h;
                }
            }

            ret = ret + 1;
        }

        return ret;
    }

    //用来获取以node为根节点的树的度数（孩子数量），根节点的度数为子树度数的最大值
    int degree(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            ret = node->child.length(); //根节点的度数

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int d = degree(node->child.current());

                if(ret < d)
                {
                    ret = d;
                }
            }
        }

        return ret;
    }
};

}


#endif // GTREE_H
