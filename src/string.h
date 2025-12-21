#pragma once
#include "dynarray.h"
#include <iostream>

class String
{
private:
	DynArray<char>* string;

	DynArray<char>* readFromFile(const char* filename)
	{
		DynArray<char>* tempstr = new DynArray<char>();
		char buffer[256];
		FILE* fp = fopen(filename, "r");
		if (fp)
		{
			while ((fgets(buffer, 256, fp)) != NULL)
			{
				for (int i = 0; (i < 256) && (buffer[i] != NULL); i++)
					tempstr->push_back(buffer[i]);
				for (int i = 0; i < 256; i++)
					buffer[i] = NULL;
			}
			fclose;
			return tempstr;
		}
		else
		{
			delete tempstr;
			std::cerr << "Ошибка чтения файла";
		}
	}

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
		string = new DynArray<char>(str2->size() * 2);
		this->addString(str2);
	}

	String(const char* filename)
	{
		string = readFromFile(filename);
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

	void addString(String* str2)
	{
		int str2Size = str2->size();
		for (int i = 0; i < str2Size; i++)
			string->push_back((*str2)[i]);
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

	void addMultiChar(const char* charStr)
	{
		for (int i = 0; (i < 4000) && (charStr[i] != '\0'); i++)
			string->push_back(charStr[i]);
	}

	bool operator ==(String& str2)
	{
		int strsize = this->size();
		if (strsize != str2.size())
			return false;
		for (int i = 0; i < strsize; i++)
		{
			if (string[i] != str2[i])
				return false;
		}
		return true;
	}

	char* toChar()
	{
		return string->toMas();
	}

	bool operator ==(const char* charStr) // сравнение первых элементов
	{
		int i = 0;
		bool lastIsEnd = false;
		if (charStr == nullptr)
			return false;
		for (i; i < string->size(); i++)
		{
			if (lastIsEnd)
				return false;
			if (charStr[i] == '\0')
				lastIsEnd;
			if ((*string)[i] != charStr[i])
				return false;
		}
		return true;
	}

	bool writeToFile(const char* filename)
	{
		FILE* fp = fopen(filename, "w");
		if (fp)
		{
			int strLen = string->size();
			char* outCharList = string->toMas();
			for (int i = 0; i < strLen; i++)
				fputc(outCharList[i], fp);
			fclose;
			return true;
		}
		else
		{
			std::cerr << "Ошибка записи в файл";
			return false;
		}
	}
	//std::string toString()
	//{
	//	std::string str1;
	//	for (int i = 0; i < this->size(); i++)
	//		str1.append(std::to_string((*this)[i]));
	//	return str1;
	//}
};

std::ostream& operator <<(std::ostream& out, String& str)
{
	int strsize = str.size();
	for (int i = 0; i < strsize; i++)
		out << str[i];
	return out;
}