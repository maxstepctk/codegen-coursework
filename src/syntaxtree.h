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



	bool readFromFile(const char* filename)
	{
		Stack<SyntaxTree*> s1;
		String* readedStr = new String(filename);
		if (readedStr == nullptr)
			return false;
		else
		{
			s1.push(this);
			String* tempStr = new String();
			int readedStrSize = readedStr->size();
			bool isStart = true;
			bool lastIsClose = false;
			for (int i = 0; i < readedStrSize; i++)
			{
				SyntaxTree* editingTree = s1.top();
				char letter = (*readedStr)[i];
				if (letter == '(')
				{
					if (!isStart)
					{
						if (lastIsClose)
						{
							editingTree->right = new SyntaxTree();
							s1.push(editingTree->right);
						}
						else
						{
							editingTree->info = new String(tempStr);
							delete tempStr;
							tempStr = new String();
							editingTree->left = new SyntaxTree();
							s1.push(editingTree->left);
						}
					}
					isStart = false;
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
					lastIsClose = true;
				}
				else
				{
					tempStr->addSym(letter);
					lastIsClose = false;
				}
			}
		}
	}

	String* printsubtree(const SyntaxTree* tree)
	{
		String* str1 = new String();
		str1->addString(tree->info);
		if (tree->left != nullptr)
		{
			str1->addSym('(');
			String* fromLeft = printsubtree(tree->left);
			str1->addString(fromLeft);
			delete fromLeft;
			str1->addSym(')');
		}
		if (tree->right != nullptr)
		{
			str1->addSym('(');
			String* fromRight = printsubtree(tree->right);
			str1->addString(fromRight);
			delete fromRight;
			str1->addSym(')');
		}
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