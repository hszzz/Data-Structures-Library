#ifndef GRAPH_H
#define GRAPH_H
#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"

namespace HsTL
{

template <class E>
struct Edge : public Object
{
    int b;
    int e;
    E data;

    Edge(int i=-1, int j=-1)
    {
        b = i;
        e = j;
    }
    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data = value;
    }

    //比较起点和终点是否相同
    bool operator == (const Edge<E>& obj) const
    {
        return ((b == obj.b) && (e == obj.e));
    }

     bool operator != (const Edge<E>& obj) const
     {
         return !(*this == obj);
     }

     bool operator < (const Edge<E>& obj) const
     {
         return (data < obj.data);
     }

     bool operator > (const Edge<E>& obj) const
     {
         return (data > obj.data);
     }
};

template <class V, class E> //与顶点相关联的数据元素类型和与边相关联的数据类型
class Graph : public Object
{
public:
    virtual V getVertex(int i) = 0;
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;
    virtual SharedPointer<Array<int>> getAdjacent(int i) = 0;
    virtual E getEdge(int i, int j) = 0;
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;
    virtual bool removeEdge(int i, int j) = 0;

    virtual int vCount() = 0;
    virtual int eCount() = 0;
    virtual int OD(int i) = 0;
    virtual int ID(int i) = 0;
    virtual int TD(int i)
    {
        return ID(i) + OD(i);
    }

    //判断在d当前图中顶点i到顶点j是否邻接
    virtual bool isAdjacent(int i, int j) = 0;

    //判断当前有向图是否能够看作无向图
    bool asUndirected()
    {
        bool ret = true;

        for(int i=0; i<vCount(); i++)
        {
            for(int j=0; j<vCount(); j++)
            {
                if(isAdjacent(i, j))
                {
                    ret = ret && isAdjacent(j, i) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }

        return ret;
    }

    //以i为起始顶点，广度优先遍历
    SharedPointer<Array<int>> BFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vCount()))
        {
            LinkQueue<int> q;
            LinkQueue<int> r;                           //ret
            DynamicArray<bool> visited(vCount());       //标记已经访问到的顶点

            for(int i=0; i<visited.length(); i++)
            {
                visited[i] = false;
            }

            q.add(i);

            while (q.length() > 0)
            {
                int v = q.front();

                q.remove();

                if(!visited[v])
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v); //获取v结点的可到达的元素，并压入队列

                    for(int j=0; j<aj->length(); j++)
                    {
                        q.add((*aj)[j]);
                    }

                    //将该结点压入返回队列里
                    r.add(v);
                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    //以i为起始顶点，深度优先遍历
    SharedPointer<Array<int>> DFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vCount()))
        {
            LinkStack<int> s;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vCount());

            for(int j=0; j<visited.length(); j++)
            {
                visited[j] = false;
            }

            s.push(i);

            while (s.size() > 0)
            {
                int v = s.top();
                s.pop();

                if(!visited[v])
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v);

                    for(int j=aj->length()-1; j>=0; j--)
                    {
                        s.push((*aj)[j]);
                    }

                    r.add(v);
                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i Is Invalid ...");
        }

        return ret;
    }

    //prim（以顶点为核心）求最小生成树，返回一个数组用于表明选中的边
    SharedPointer< Array< Edge<E> > > prim(const E& LIMIT/*理论上最大(小)权值*/, const bool MINIMUM = true/*选择最大（最小）生成树*/)
    {
        LinkQueue<Edge<E>> ret;                     //记录最小生成树的边

        if(asUndirected())                          //只能求无向图的最小生成树
        {
            DynamicArray<int> adjVex(vCount());     //记录cost中权值所对应的顶点
            DynamicArray<bool> mark(vCount());      //标记顶点所属的结合，便于确定最小边
            DynamicArray<E> cost(vCount());         //记录最小权值

            SharedPointer<Array<int>> aj = NULL;
            bool end = false;                       //判断prim是否中断执行
            int v = 0;                              //从0顶点来获取最小生成树

            for(int i=0; i<vCount(); i++)           //init
            {
                adjVex[i] = -1;
                mark[i] = false;
                cost[i] = LIMIT;
            }

            mark[v] = true;

            aj = getAdjacent(v);

            for(int j=0; j<aj->length(); j++)
            {
                cost[(*aj)[j]] = getEdge(v, (*aj)[j]);
                adjVex[(*aj)[j]] = v;
            }

            for(int i=0; (i<vCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1;

                for(int j=0; j<vCount(); j++)
                {
                    if(!mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1);

                if(!end)
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));

                    mark[k] = true;

                    aj = getAdjacent(k);

                    for(int j=0; j<aj->length(); j++)
                    {
                        if(!mark[(*aj)[j]] && (MINIMUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : (getEdge(k, (*aj)[j]) > cost[(*aj)[j]])))
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }

        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "prim Is For Undirected Graph Only ...");
        }

        if(ret.length() != (vCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No Enough Edge For prim ...");
        }

        return toArray(ret);
    }

    //kruskal（以边为核心）求最小生成树，返回一个数组用于表明选中的边
    SharedPointer<Array<Edge<E>>> kruskal(const bool MINMUM = true)
    {
        LinkQueue<Edge<E>> ret;
        SharedPointer<Array<Edge<E>>> edges = getUndirectedEdges();
        DynamicArray<int> p(vCount());                              //前驱标记数组

        for(int i=0; i<p.length(); i++)                             //init
        {
            p[i] = -1;
        }

        Sort::Shell(*edges, MINMUM);                                        //按权值对边进行排序

        //选边 最多循环length次，并且如果有vcount-1个边了，就停止循环
        for(int i=0; (i<edges->length()) && (ret.length() < (vCount()-1)); i++)
        {
            int b = find(p, (*edges)[i].b);                         //b == e，则不能选这条边，会构成回路
            int e = find(p, (*edges)[i].e);

            if(b != e)
            {
                p[e] = b;                                           //修改前驱标记数组

                ret.add((*edges)[i]);
            }
        }

        if(ret.length() != (vCount() - 1))                          //最小生成树的边必须是顶点-1个
        {
            THROW_EXCEPTION(InvalidOperationException, "No Enough Edges For kruskal ...");
        }

        return toArray(ret);
    }

    //最短路径 Dijkstra算法 : 由已知最短路径得到未知最短路径
    SharedPointer<Array<int>> dijkstra(int i, int j, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()))
        {
            DynamicArray<E> dist(vCount());                         //用于存储路径的值
            DynamicArray<int> path(vCount());                       //用于存储当前结点的前驱结点
            DynamicArray<bool> mark(vCount());                      //标记顶点是否已经算好了

            for(int k=0; k<vCount(); k++)
            {
                mark[k] = false;
                path[k] = -1;
                dist[k] = isAdjacent(i, k) ? (path[k]=i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true;

            for(int k=0; k<vCount(); k++)
            {
                E m = LIMIT;
                int u = -1;

                for(int w=0; w<vCount(); w++)
                {
                    if(!mark[w] && (dist[w] < m))
                    {
                        m = dist[w];
                        u = w;
                    }
                }

                if(u == -1)
                {
                    break;
                }

                mark[u] = true;

                for(int w=0; w<vCount(); w++)
                {
                    if(!mark[w] && isAdjacent(u, w) && ((dist[u] + getEdge(u, w)) < dist[w]))
                    {
                        dist[w] = dist[u] + getEdge(u, w);
                        path[w] = u;
                    }
                }
            }

            LinkStack<int> s;

            s.push(j);

            for(int k=path[j]; k!=-1; k=path[k])
            {
                s.push(k);
            }

            while (s.size() > 0)
            {
                ret.add(s.top());

                s.pop();
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i or j Is Invalid ...");
        }

        if(ret.length() < 2)        //路径至少有两个顶点，否则i和j不可达
        {
            THROW_EXCEPTION(ArithmeticException, "There Is No Path From i to j ...");
        }

        return toArray(ret);
    }

    //最短路径 Floyd算法
    SharedPointer<Array<int>> floyd(int x, int y, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= x) && (x < vCount()) && (0 <= y) && (y < vCount()))
        {
            //二维数组及其初始化
            DynamicArray< DynamicArray<E> > dist(vCount());
            DynamicArray<DynamicArray<int>> path(vCount());

            for(int k=0; k<vCount(); k++)
            {
                dist[k].resize(vCount());
                path[k].resize(vCount());
            }

            for(int i=0; i<vCount(); i++)
            {
                for(int j=0; j<vCount(); j++)
                {
                    path[i][j] = -1;

                    dist[i][j] = isAdjacent(i, j) ? (path[i][j]=j, getEdge(i, j)) : LIMIT;
                }
            }

            //核心算法
            for(int k=0; k<vCount(); k++)
            {
                for(int i=0; i<vCount(); i++)
                {
                    for(int j=0; j<vCount(); j++)
                    {
                        if((dist[i][k] + dist[k][j]) < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k];
                        }
                    }
                }
            }

            while ((x != -1) && (x != y))
            {
                ret.add(x);
                x = path[x][y];
            }

            if(x != -1)
            {
                ret.add(x);
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index x or y Is Invalid ...");
        }

        if(ret.length() < 2)        //路径至少有两个顶点，否则i和j不可达
        {
            THROW_EXCEPTION(ArithmeticException, "There Is No Path From x to y ...");
        }

        return toArray(ret);
    }

protected:
    //将队列转化为数组
    template<typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if(ret != NULL)
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front());
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create ret Obj ...");
        }

        return ret;
    }

    //获取无向图的所有边
    SharedPointer<Array<Edge<E>>> getUndirectedEdges()
    {
        DynamicArray<Edge<E>>* ret = NULL;

        if(asUndirected())
        {
            LinkQueue<Edge<E>> queue;
            for(int i=0; i<vCount(); i++)
            {
                for(int j=i; j<vCount(); j++)
                {
                    if(isAdjacent(i, j))
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }

            ret = toArray(queue);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "This Func Is For undirected Graph only ...");
        }


        return ret;
    }

    //查找前驱标记数组
    int find(Array<int>& p, int v)
    {
        while (p[v] != -1)
        {
            v = p[v];
        }
        return v;
    }
};


}


#endif // GRAPH_H
