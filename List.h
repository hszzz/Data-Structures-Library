#ifndef LIST_HPP
#define LIST_HPP
#include "HsObject.h"
namespace HsTL
{
template <class T>
class List : public HsObject
{
public:
    virtual bool insert(int i, const T& e) = 0;
    virtual bool remove(int i) = 0;
    virtual bool set(int i, const T& e) = 0;
    virtual bool get(int i, T& e) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
};

}
#endif // LIST_HPP
