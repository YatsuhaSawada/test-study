#include "stack.hpp"

#include <algorithm>

const int SIZE_I = 8;

Stack::Stack()
    : _capacity(SIZE_I)
    , _count(0)
    , _pBody(new int[_capacity])
{    
}

Stack::~Stack()
{
    delete[] _pBody;
}

bool Stack::push(int v)
{
    if (_capacity == _count) {
        _capacity = _capacity * 2;
        
        int* p = new(std::nothrow) int[_capacity];
        if (p == nullptr) {
            return false;
        }

         std::copy(&_pBody[0], &_pBody[_count], &p[0]);

         delete _pBody;
         _pBody = p;
    }

    _pBody[_count++] = v;

    return true;
}

bool Stack::pop(int& v)
{
    if (_count < 1){
        return false;
    }

    v = _pBody[--_count];

    return true;
}

int Stack::count() const
{
    return _count;
}

int* Stack::GetBody(){
    return _pBody;
}

int Stack::GetCount(){
    return _count;
}