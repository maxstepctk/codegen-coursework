#pragma once

#include <iostream>

template <typename T>
class DynArray
{
private:
    T* array;
    T* temparray = nullptr;
    int sizearr = 0;
    int capacity = 5;

public:
    DynArray()
    {
        array = new T[capacity];
    }

    DynArray(int neededCapacity)
    {
        capacity = neededCapacity;
        array = new T[neededCapacity];
    }

    DynArray(const DynArray<T>& arr2)
    {
        int size2 = arr2.sizearr;
        sizearr = size2;
        capacity = size2 * 2;
        array = new T[capacity];
        for (int i = 0; i < size2; i++)
            array[i] = arr2.array[i];
    }

    ~DynArray()
    {
        delete[] array;
    }

    T& operator [](int n)
    {
        if (n >= sizearr)
            std::cerr << "n больше размера массива\n";
        return array[n];
    }

    bool operator ==(const DynArray<T>& arr2)
    {

        for (int i = 0; i < sizearr; i++)
        {
            if (array[i] != arr2.array[i])
                return false;
        }
        return true;
    }

    int size()
    {
        return sizearr;
    }

    bool empty()
    {
        return (sizearr == 0);
    }

    void reserve(int cap)
    {
        if (cap >= sizearr)
        {
            capacity = cap;
            temparray = new T[capacity];
            for (int i = 0; i < sizearr; i++)
                temparray[i] = array[i];
            delete[] array;
            array = temparray;
        }
        else
            std::cerr << "Выделяемый объём памяти меньше существующего массива";
    }

    void push_back(T elem)
    {
        if (sizearr != capacity)
        {
            array[sizearr++] = elem;
        }
        else
        {
            reserve(capacity * 2);
            array[sizearr++] = elem;
        }
    }

    void pop_back()
    {
        if (sizearr > 0)
            sizearr--;
    }

    T max()
    {
        T maxelem = array[0];
        for (int i = 1; i < sizearr; i++)
        {
            if (array[i] > maxelem)
                maxelem = array[i];
        }
        return maxelem;
    }

    T min()
    {
        T minelem = array[0];
        for (int i = 1; i < sizearr; i++)
        {
            if (array[i] < minelem)
                minelem = array[i];
        }
        return minelem;
    }

};