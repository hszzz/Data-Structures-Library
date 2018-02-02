#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include "Array.h"
#include "Exception.h"

namespace HsTL
{
template <class T>
class DynamicArray : public Array<T>
{
public:
    DynamicArray(int length)
    {
        init(new T[length], length);
        /*
        this->m_array = new T[length];

        //判断内存是否申请成功
        if(this->m_array != NULL)
        {
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To Create DynamicArray ...");
        }
        */
    }

    DynamicArray(const DynamicArray<T>& obj)
    {
        init(copy(obj.m_array, obj.m_length, obj.m_length),obj.m_length);
        /*
        this->m_array = new T[obj.m_length];

        //判断内存是否申请成功
        if(this->m_array != NULL)
        {
            this->m_length = obj.m_length;

            for(int i=0; i<obj.m_length; i++)
            {
                this->m_array[i] = obj.m_array[i];
            }

        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create DynamicArray ...");
        }
        */
    }

    DynamicArray<T>& operator= (const DynamicArray<T>& obj)
    {        
        //自赋值判断
        if(this != &obj)
        {
            update(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);
            /*
            T* array = new T[obj.m_length];

            if(array != NULL)
            {
                for(int i=0; i<obj.m_length; i++)
                {
                    array[i] = obj.m_array[i];
                }

                //以下代码任然是为了异常安全
                T* tmp = this->m_array;

                this->m_array = array;
                this->m_length = obj.m_length;

                delete[] tmp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To copy DynamicArray ...");
            }
            */
        }

        return *this;
    }

    int length() const
    {
        return m_length;
    }

    void resize(int length)
    {
        if(length != m_length)
        {
            update(copy(this->m_array, m_length, length), length);
            /*
            T* array = new T[length];

            if(array != NULL)
            {
                //取最小的长度，避免越界操作
                int size = (length < m_length) ? length : m_length;

                for(int i=0; i<size; i++)
                {
                    array[i] = this->m_array[i];
                }

                //异常安全
                T* tmp = this->m_array;

                this->m_array = array;
                this->m_length = length;

                delete[] tmp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To resize DynamicArray ...");
            }
            */
        }
    }

    ~DynamicArray()
    {
        delete[] this->m_array;
    }

protected:
    int m_length;

    //此函数用于将一个长度为len的数组（array），copy到一个新的长度为newLen长度的数组中
    T* copy(T* array, int len, int newLen)
    {
        T* ret = new T[newLen];

        if(ret != NULL)
        {
            int size = (len < newLen) ? len : newLen;

            for(int i=0; i<size; i++)
            {
                ret[i] = array[i];
            }
        }
        return ret;
    }

    //此函数用于 异常安全地 更新线性表
    void update(T* array, int length)
    {
        if(array != NULL)
        {
            T* tmp = this->m_array;

            this->m_array = array;
            this->m_length = length;

            delete[] tmp;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To update DynamicArray ...");
        }
    }

    //用于初始化线性表
    void init(T* array, int length)
    {
        if(array != NULL)
        {
            this->m_array =array;
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Memory To create DynamicArray ...");
        }
    }
};
}
#endif // DYNAMICARRAY_H
