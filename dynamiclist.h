#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H
#include "SeqList.h"
#include "Exception.h"
namespace HsTL
{
template <class T>
class DynamicList : public SeqList<T>
{
public:
    DynamicList(int capacity)
    {
        this->m_array = new T[capacity];
        if(this->m_array != NULL)
        {
            this->m_length = 0;
            this->m_capacity = capacity;
        }
        else
        {
           THROW_EXCEPTION(NoEnoughMemoryException,"No Memory To Create DynamicList ...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    void resize(int capacity)
    {
        if(capacity != m_capacity)
        {
            T* array = new T[capacity];

            if(array != NULL)
            {
                //为把旧空间里的数据copy到新空间做准备 -> 确定该copy多少的数据
                //总是按照最小的那个算，确保不会越界访问
                int length = (this->length()) < capacity ? this->m_length : capacity;

                //copying...
                for(int i=0; i<length; i++)
                {
                    array[i] = this->m_array[i];
                }

                //copy完成，新旧空间交换，析构旧空间
                T* tmp = this->m_array; //如果delete[] this->m_array; 这样写
                                        //不能保证此函数的异常安全
                                        //如果T为一个类类型，delete会触发析构函数，可能有异常抛出
                                        //下面的代码得不到执行，无法保证此线性表还可用
                this->m_array = array;
                this->m_length = length;
                this->m_capacity = capacity;

                delete[] tmp;           //即便在这里有异常返回，但是上面的赋值语句已经完成，线性表任然可用
            }
            else //空间申请失败就抛出异常
            {
                THROW_EXCEPTION(NoEnoughMemoryException,"No Memory To Resize DynamicList ...");
            }

        }
    }

    ~DynamicList()
    {
        delete[] this->m_array;
    }

protected:
    int m_capacity;
};
}
#endif // DYNAMICLIST_H
