#ifndef LIST_HPP
#define LIST_HPP
#include "Object.h"
namespace HsTL
{
template <class T>
class List : public Object
{
public:
    List() {}//!!!
    virtual bool insert(const T& e) = 0;
    virtual bool insert(int index, const T& e) = 0;
    virtual bool remove(int index) = 0;
    virtual bool set(int index, const T& e) = 0;
    virtual bool get(int index, T& e) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual int find(const T& e) const = 0;
protected:
    //容器禁止copy操作和赋值操作
    T operator= (const List&) {}
    List(const List&) {}
};

}
#endif // LIST_HPP
