#pragma once

#include "binarytree.h"
#include "string.h"
#include "stack.h"
#include <Windows.h>

class SyntaxTree : public BinaryTree<String>
{
protected:
	SyntaxTree* left = nullptr;
	SyntaxTree* right = nullptr;
public:
	SyntaxTree() : BinaryTree<String>() {}

	SyntaxTree(String data) : BinaryTree<String>(data) {};

	SyntaxTree(const char* filename)
	{
		info = nullptr;
		sizetree = 0;
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
		Stack<SyntaxTree*> s1;
		std::cout << "Адрес этого дерева: " << this << std::endl;
		//int targetStackLevel = 1;
		String* readedStr = readStringFromFile(filename);
		if (readedStr == nullptr)
			return false;
		else
		{
			s1.push(this);
			std::cout << s1.top()->info << std::endl;
			String* tempStr = new String();
			int readedStrSize = readedStr->size();
			bool isStart = true;
			bool lastIsClose = false;
			for (int i = 0; i < readedStrSize; i++)
			{
				SyntaxTree* editingTree = s1.top();
				char letter = (*readedStr)[i];
				std::cout << letter << std::endl;
				if (letter == '(')
				{
					isStart = false;
					if (!isStart)
					{
						editingTree->info = new String(tempStr);
						delete tempStr;
						tempStr = new String();
						if (lastIsClose)
						{
							editingTree->right = new SyntaxTree();
							s1.push(editingTree->right);
						}
						else
						{
							editingTree->left = new SyntaxTree();
							s1.push(editingTree->left);
						}
					}
					lastIsClose = false;
				}
				else if (letter == ')')
				{
					if (!lastIsClose)
					{
						editingTree->info = new String(tempStr);
						delete tempStr;
						tempStr = new String();
					}
					s1.pop();
					std::cout << "Размер стека: " << s1.size() << std::endl;
					lastIsClose = true;
				}
				else
					tempStr->addSym(letter);
			}
		}
	}

	String* printsubtree(const SyntaxTree* tree)
	{
		String* str1 = new String();
		str1->addString(tree->info);
		str1->addSym('(');
		if (tree->left != nullptr)
		{
			String* fromLeft = printsubtree(tree->left);
			str1->addString(fromLeft);
			delete fromLeft;
		}
		str1->addSym(')');
		str1->addSym('(');
		if (tree->right != nullptr)
		{
			String* fromRight = printsubtree(tree->right);
			str1->addString(fromRight);
			delete fromRight;
		}
		str1->addSym(')');
		return str1;
	}
};

std::ostream& operator <<(std::ostream& out, SyntaxTree& tree)
{
	String* result = tree.printsubtree(&tree);
	String toOut(result);
	delete result;
	out << toOut;
    return out;
}