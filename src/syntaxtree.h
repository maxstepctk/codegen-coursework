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
		s1.push(this);
		String* readedstr = readStringFromFile(filename);
		if (readedstr == nullptr)
			return false;
		else
		{
			bool isStart = true;
			String* tempstr = nullptr;
			std::cout << *readedstr << std::endl;
			info = new String(*readedstr);
			//for (int i = 0; i < readedstr->size(); i++)
			//{
			//	std::cout << i << std::endl;
			//	SyntaxTree* editingTree = s1.top();
			//	std::cout << *readedstr << std::endl;
		//		if ((*readedstr)[i] == '(')
		//		{
		//			if (editingTree->info == nullptr)
		//			{
		//				if (isStart == false)
		//				{
		//					info = new String(tempstr);
		//					delete tempstr;
		//					left = new SyntaxTree;
		//					s1.push(left);
		//				}
		//				tempstr = new String();
		//			}
		//			else
		//			{
		//				right = new SyntaxTree;
		//				s1.push(right);
		//			}
		//		}
		//		else if ((*readedstr)[i] == ')')
		//		{
		//			info = new String(tempstr);
		//			delete tempstr;
		//			tempstr = new String();
		//			s1.pop();
		//		}
		//		else
		//			tempstr->addSym((*readedstr)[i]);
		//	}
		}
	}
	std::string printsubtree(const SyntaxTree* tree)
	{
		std::string str1;
		str1.append(tree->info->toString());
		str1.append("(");
		if (tree->left != nullptr)
			str1.append(printsubtree(tree->left));
		str1.append(")(");
		if (tree->right != nullptr)
			str1.append(printsubtree(tree->right));
		str1.append(")");
		return str1;
	}
};

std::ostream& operator <<(std::ostream& out, SyntaxTree& tree)
{
    out << tree.printsubtree(&tree);
    return out;
}