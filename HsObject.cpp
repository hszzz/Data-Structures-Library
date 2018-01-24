#include "HsObject.h"
#include <cstdlib>
#include <iostream>
namespace HsTL
{

void* HsObject::operator new(unsigned int size) throw()
{
    return malloc(size);
}

void HsObject::operator delete(void* p)
{
    free(p);
}

void* HsObject::operator new[](unsigned int size) throw()
{
    return malloc(size);
}

void HsObject::operator delete[](void* p)
{
    free(p);
}

HsObject::~HsObject()
{

}
}

