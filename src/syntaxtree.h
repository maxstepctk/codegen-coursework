#pragma once

#include "binarytree.h"
#include "string.h"
#include "stack.h"
#include <Windows.h>

class SyntaxTree : public BinaryTree<String>
{
public:
	SyntaxTree() : BinaryTree<String>() {}

	SyntaxTree(String data) : BinaryTree<String>(data) {};

	SyntaxTree(const char* filename)
	{
		readFromFile(filename);
	}

	~SyntaxTree() {};

	String* readStringFromFile(const char* filename)
	{
		String* tempstr = new String();
		char buffer[256];
		FILE* fp = fopen(filename, "r");
		if (fp)
		{
			while ((fgets(buffer, 256, fp)) != NULL)
			{
				for (int i = 0; (i < 256) && (buffer[i] != NULL); i++)
					tempstr->addSym(buffer[i]);
				for (int i = 0; i < 256; i++)
					buffer[i] = NULL;
			}
			return tempstr;
		}
		else
		{
			delete tempstr;
			std::cerr << "Ошибка чтения файла";
		}
	}

	bool readFromFile(const char* filename)
	{
		Stack<BinaryTree<String>*> s1;
		s1.push(this);
		String* readedstr = readStringFromFile(filename);
		if (readedstr == nullptr)
			return false;
		else
		{
			std::cout << *readedstr << std::endl;
		}
			//	if (*letter == '(')
			//	{
			//		if (notStart)
			//		{
			//			s1.top()->writeToHead(*tempstr);
			//			if (tempstr != nullptr)
			//				delete tempstr;
			//			tempstr = new String();
			//			if (lastIsClose)
			//				s1.push(s1.top()->right);
			//			else
			//				s1.push(s1.top()->left);
			//		}
			//	}
			//	else if (*letter = ')')
			//	{
			//		s1.top()->writeToHead(*tempstr);
			//		delete tempstr;
			//		s1.pop();
			//	}
			//	else
			//		tempstr->addSym(*letter);
	}
};