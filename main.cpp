#include <iostream>
#include "DynamicList.h"
using namespace std;
using namespace HsTL;

int main()
{
    DynamicList<int> dl(3);
    dl.insert(0,1);
    dl.insert(0,2);
    dl.insert(0,3);

    for(int i=0; i<dl.capacity(); i++)
    {
        cout<< dl[i] <<endl;
    }

    return 0;
}
