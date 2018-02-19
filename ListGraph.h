#ifndef LISTGRAPH_H
#define LISTGRAPH_H
/**********************************
 * 图的邻接链表表示
 * 空间占用低，但性能差
 * 18-2-12
***********************************/
#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace HsTL
{

template <class V, class E>
class ListGraph : public Graph<V, E>
{
public:
    ListGraph(unsigned int n = 0/*n个顶点*/)
    {
        for(unsigned int i=0; i<n; i++)
        {
            addVertex();
        }
    }

    //添加顶点，只能在最末端添加，否则会影响其他顶点的编号
    int addVertex()
    {
        int ret = -1;

        Vertex* v = new Vertex();

        if(v != NULL)
        {
            m_list.insert(v);
            ret = m_list.length() - 1;      //获取顶点的编号（顶点添加只能从后面添加）
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New vertex Obj ...");
        }

        return ret;
    }

    //添加顶点，并添加顶点所对应的值
    int addVertex(const V& value)
    {
        int ret = addVertex();

        if(ret >= 0)
        {
            setVertex(ret, value);
        }

        return ret;
    }

    //设置i顶点的元素为value
    bool setVertex(int i, const V &value)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            V* data = vertex->data;

            if(data == NULL)
            {
                data = new V();
            }

            if(data != NULL)
            {
                *data = value;
                vertex->data = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create New vertex Value ...");
            }
        }

        return ret;
    }

    //将i处的顶点的元素返回
    V getVertex(int i)
    {
        V ret;

        if(!getVertex(i, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    bool getVertex(int i, V& value)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if(ret)
        {
            Vertex* v = m_list.get(i);

            if(v->data != NULL)
            {
                value = *(v->data);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No Value Assigned To this Vertex ...");
            }
        }

        return ret;
    }

    //删除最近添加的顶点
    void removeVertex()
    {
        if(m_list.length() > 0)
        {
            int index = m_list.length() - 1;
            Vertex* v = m_list.get(index);

            //删除对应结点
            if(m_list.remove(index))
            {
                //删除该节点所有相关的边
                for(int i =(m_list.move(0), 0); !m_list.end(); i++,m_list.next())
                {   //i为遍历的结点所对应的编号
                    int pos = m_list.current()->edge.find(Edge<E>(i, index)/*匿名对象*/);//i为起点，index为终点的边

                    if(pos >= 0)
                    {
                        m_list.current()->edge.remove(pos);
                    }
                }

                delete v->data;
                delete v;
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No vertex In Current Graph ...");
        }
    }

    //获取从顶点i出发，可到达的顶点的编号
    SharedPointer<Array<int>> getAdjacent(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vCount()))
        {
            Vertex* vertex = m_list.get(i);

            ret = new DynamicArray<int>(vertex->edge.length());

            if(ret != NULL)
            {
                for(int k=(vertex->edge.move(0), 0); !vertex->edge.end(); k++, vertex->edge.next())
                {
                    ret->set(k, vertex->edge.current().e);
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create ret Array Obj ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    //获取i和j之间的权值
    E getEdge(int i, int j)
    {
        E ret;

        if(!getEdge(i, j, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Edge<i, j> Is Invalid ...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                value = vertex->edge.get(pos).data;
            }
            else
            {
                THROW_EXCEPTION(InvalidParameterException, "No value Assigned To This edge ...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.set(pos, Edge<int>(i, j, value));
            }
            else
            {
                ret = vertex->edge.insert(0, Edge<E>(i, j, value));
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j)
    {
        bool ret = ( (0 <= i) && (i < vCount()) &&
                     (0 <= j) && (j < vCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.remove(pos);
            }
        }

        return ret;
    }

    int vCount()
    {
        return m_list.length();
    }

    int eCount()
    {
        int ret = 0;

        for(m_list.move(0); !m_list.end(); m_list.next())
        {
            ret += m_list.current()->edge.length();
        }

        return ret;
    }

    int OD(int i)
    {
        int ret = 0;

        if((0 <= i) && (i < vCount()))
        {
            ret = m_list.get(i)->edge.length();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    int ID(int i)
    {
        int ret = 0;

        if((0 <= i) && (i < vCount()))
        {
            for(m_list.move(0); !m_list.end(); m_list.next())
            {
                LinkList<Edge<E>>& edge = m_list.current()->edge;

                for(edge.move(0); !edge.end(); edge.next())
                {
                    if(edge.current().e == i)
                    {
                        ret++;
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    ~ListGraph()
    {
        while (m_list.length() > 0)
        {
            Vertex* toDel = m_list.get(0);
            m_list.remove(0);

            delete toDel->data;
            delete toDel;
        }
    }

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0);
    }

protected:
    struct Vertex : public Object
    {
        V* data;
        LinkList<Edge<E>> edge;
        Vertex()
        {
            data = NULL;
        }
    };

    LinkList<Vertex*> m_list;
};


}

#endif // LISTGRAPH_H
