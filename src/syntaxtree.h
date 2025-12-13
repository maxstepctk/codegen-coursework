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

	bool readFromFile(const char* filename)
	{
		Stack<SyntaxTree*> s1;
		String* readedStr = new String(filename);
		if (readedStr == nullptr)
			return false;
		else
		{
			s1.push(this);
			String* tempStrName = new String();
			String* tempStrValue = new String();
			int readedStrSize = readedStr->size();
			bool isStart = true;
			bool lastIsClose = false;
			bool beforeValue = true;
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
							editingTree->name = new String(tempStrName);
							editingTree->value = new String(tempStrValue);
							delete tempStrName;
							delete tempStrValue;
							tempStrName = new String();
							tempStrValue = new String();
							editingTree->left = new SyntaxTree();
							s1.push(editingTree->left);
						}
					}
					isStart = false;
					lastIsClose = false;
					beforeValue = true;
				}
				else if (letter == ')')
				{
					if (!lastIsClose)
					{
						editingTree->name = new String(tempStrName);
						editingTree->value = new String(tempStrValue);
						delete tempStrName;
						delete tempStrValue;
						tempStrName = new String();
						tempStrValue = new String();
						beforeValue = true;
					}
					s1.pop();
					lastIsClose = true;
				}
				else
				{
					if ((letter == ':') && beforeValue)
						beforeValue = false;
					else
					{
						if (beforeValue)
							tempStrName->addSym(letter);
						else
							tempStrValue->addSym(letter);
					}
					lastIsClose = false;
				}
			}
		}
	}

public:

	SyntaxTree() : BinaryTree<String>() {}

	SyntaxTree(String data) : BinaryTree<String>(data) {};

	SyntaxTree(const char* filename)
	{
		name = nullptr;
		value = nullptr;
		sizetree = 0;
		readFromFile(filename);
	}

	~SyntaxTree() {};

	String* printsubtree(const SyntaxTree* tree)
	{
		String* str1 = new String();
		str1->addString(tree->name);
		if (!tree->value->isEmpty())
		{
			str1->addSym(':');
			str1->addString(tree->value);
		}

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

	String* printSubtreeIndent(const SyntaxTree* tree, int level)
	{
		String* str1 = new String();
		str1->addString(tree->name);
		if (!tree->value->isEmpty())
		{
			str1->addSym(':');
			str1->addString(tree->value);
		}
		level++;

		if (tree->left != nullptr)
		{
			str1->addSym('\n');
			for (int i = 0; i < level; i++)
				str1->addSym('-');
			String* fromLeft = printSubtreeIndent(tree->left, level);
			str1->addString(fromLeft);
			delete fromLeft;
		}
		if (tree->right != nullptr)
		{
			str1->addSym('\n');
			for (int i = 0; i < level; i++)
				str1->addSym('-');
			String* fromRight = printSubtreeIndent(tree->right, level);
			str1->addString(fromRight);
			delete fromRight;
		}
		return str1;
	}
};

std::ostream& operator <<(std::ostream& out, SyntaxTree& tree)
{
	String* result = tree.printSubtreeIndent(&tree, 0);
	String toOut(result);
	delete result;
	out << toOut;
    return out;
}