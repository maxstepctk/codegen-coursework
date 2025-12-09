#pragma once
#include "dynarray.h"

template <typename T>
class Stack
{
private:
    DynArray<T>* stck1;
public:
    Stack()
    {
        stck1 = new DynArray<T>();
    }
    ~Stack()
    {
        delete stck1;
    }
    int size()
    {
        return stck1->size();
    }
    bool empty()
    {
        return stck1->empty();
    }
    T& top()
    {
        return (*stck1)[stck1->size() - 1];
    }
    void push(const T data)
    {
        stck1->push_back(data);
    }
    void pop()
    {
        stck1->pop_back();
    }
};
