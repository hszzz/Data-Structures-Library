#ifndef QUEUE_H
#define QUEUE_H
#include "Object.h"

namespace HsTL
{

template <class T>
class Queue : public Object
{
public:
    virtual void add(const T& e) = 0; //入队
    virtual void remove() = 0;        //出队
    virtual T front() const = 0;      //获取队首元素
    virtual void clear() = 0;
    virtual int length() const = 0;
};

}
#endif // QUEUE_H
