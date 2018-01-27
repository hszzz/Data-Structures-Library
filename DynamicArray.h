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
    }

    DynamicArray(const DynamicArray<T>& obj)
    {
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
    }

    DynamicArray<T>& operator= (const DynamicArray<T>& obj)
    {
        //自赋值判断
        if(this != &obj)
        {
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
        }
    }

    ~DynamicArray()
    {
        delete[] this->m_array;
    }

protected:
    int m_length;
};
}
#endif // DYNAMICARRAY_H
