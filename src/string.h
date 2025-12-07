#pragma once
#include "dynarray.h"
#include <iostream>
#include <string>

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

	String(String* str2)
	{
		string = new DynArray<char>((str2->size()) * 2);
		int str2size = str2->size();
		for (int i = 0; i < str2size; i++)
			this->addSym((*str2)[i]);
	}

	~String()
	{
		delete string;
	}

	char& operator[](int n)
	{
		return (*string)[n];
	}

	void addSym(char elem)
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

	std::string toString()
	{
		std::string str1;
		for (int i = 0; i < this->size(); i++)
			str1.append(std::to_string((*this)[i]));
		return str1;
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