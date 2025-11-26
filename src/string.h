#pragma once
#include "dynarray.h"
#include <iostream>

class String
{
private:
	DynArray<char>* string;

public:
	String()
	{
		string = new DynArray<char>();
	}

	String(int neededCapacity)
	{
		string = new DynArray<char>(neededCapacity);
	}

	~String()
	{
		delete string;
	}

	char& operator[](int n)
	{
		return (*string)[n];
	}

	void addBack(char elem)
	{
		string->push_back(elem);
	}

	void removeBack()
	{
		string->pop_back();
	}

	bool isEmpty()
	{
		return string->empty();
	}

	int size()
	{
		return string->size();
	}
};

std::ostream& operator <<(std::ostream& out, String& str)
{
	int strsize = str.size();
	for (int i = 0; i < strsize; i++)
		out << str[i];
	out << std::endl;
	return out;
}