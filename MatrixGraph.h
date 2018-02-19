#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
/****************************
 * 图的邻接矩阵方式表示
 * 空间占用太大了，但性能好
 *18-2-11
*****************************/
#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace HsTL
{
template <int N/*图中顶点个数*/, class V, class E/*权值类型*/>
class MatrixGraph : public Graph<V, E>
{
public:
    MatrixGraph()
    {
        for(int i=0; i<vCount(); i++)
        {
            m_vertexes[i] = NULL;
            for(int j=0; j<vCount(); j++)
            {
                m_edges[i][j] = NULL;
            }
        }
        m_eCount = 0;
    }

    //获取顶点的值
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
            if(m_vertexes[i] != NULL)
            {
                value = *(m_vertexes[i]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No Value Assigned To This vertex ...");
            }
        }

        return ret;
    }

    //设置顶点的值
    bool setVertex(int i, const V& value)
    {
         bool ret = ((0 <= i) && (i < vCount()));

         if(ret)
         {
             //异常安全
             V* data = m_vertexes[i];
             if(data == NULL)
             {
                data = new V();
             }

             if(data != NULL)
             {
                *data = value; //防止此处发生异常，导致对象我无法继续使用

                 m_vertexes[i] = data;
             }
             else
             {
                 THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To store New V ...");
             }
         }

         return ret;
    }

    //获取顶点i可抵达的相邻的其他顶点的编号
    SharedPointer<Array<int>> getAdjacent(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vCount()))
        {
            int n = 0;

            for(int j=0; j<vCount(); j++)
            {
                if(m_edges[i][j] != NULL)
                {
                    n++;
                }
            }

            ret = new DynamicArray<int>(n);

            if(ret != NULL)
            {
                for(int j=0, k=0; j<vCount(); j++)
                {
                    if(m_edges[i][j] != NULL)
                    {
                        ret->set(k++, j);    //记录编号
                    }
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create Ret Obj ...");
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
            THROW_EXCEPTION(InvalidParameterException, "Index <i, j> Is Invalid ...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if(ret)
        {
            if(m_edges[i][j] != NULL)
            {
                value = *(m_edges[i][j]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No Value Assigned To This edge ...");
            }
        }

        return ret;
    }

    //设置边上的权值
    bool setEdge(int i, int j, const E& value)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if(ret)
        {
            E* ne = m_edges[i][j];

            if(ne == NULL)
            {
                ne = new E();

                if(ne != NULL)
                {
                    *ne = value;
                    m_edges[i][j] = ne;
                    m_eCount++;
                }
                else
                {
                    THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To store New Edge Value ...");
                }
            }
            else
            {
                *ne = value;
            }
        }

        return ret;
    }

    //删除i和j之间的关联
    bool removeEdge(int i, int j)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()));

        if(ret)
        {
            //异常安全
            E* toDel = m_edges[i][j];

            m_edges[i][j] = NULL;

            if(toDel != NULL)
            {
                m_eCount--;

                delete toDel;
            }
        }

        return ret;
    }

    //顶点个数
    int vCount()
    {
        return N;
    }

    //边的个数
    int eCount()
    {
        return m_eCount;
    }

    //顶点i的出度
    int OD(int i)
    {
        int ret = 0;

        if((0 <= i) && (i <vCount()))
        {
            for(int j=0; j<vCount(); j++)
            {
                if(m_edges[i][j] != NULL)
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    //顶点i的入度
    int ID(int i)
    {
        int ret = 0;

        if((0 <= i) && (i <vCount()))
        {
            for(int j=0; j<vCount(); j++)
            {
                if(m_edges[j][i] != NULL)
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    ~MatrixGraph()
    {
        for(int i=0; i<vCount(); i++)
        {
            for(int j=0; j<vCount(); j++)
            {
                delete m_edges[i][j];
            }
            delete m_vertexes[i];
        }
    }

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_edges[i][j] != NULL);
    }

protected:
    V* m_vertexes[N];
    E* m_edges[N][N];
    int m_eCount;
};


}

#endif // MATRIXGRAPH_H
