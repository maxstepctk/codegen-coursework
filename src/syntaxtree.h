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
		s1.push(this);
		std::cout << s1.top()->info << std::endl;
		String* readedstr = readStringFromFile(filename);
		if (readedstr == nullptr)
			return false;
		else
		{
			bool isStart = true;
			bool lastIsClose = false;
			String* tempstr = nullptr;
			//std::cout << (*readedstr) << std::endl;
			for (int i = 0; i < readedstr->size(); i++)
			{
				std::cout << "символ " << (*readedstr)[i] << std::endl;
				//std::cout << i << std::endl;
				SyntaxTree* editingTree = s1.top();
				std::cout << "Размер стека: " << s1.size() << " Редкатируемое дерево: " << editingTree << std::endl;
				//std::cout << "инфо " << * editingTree->info << std::endl;
				//std::cout << *readedstr << std::endl;
				if ((*readedstr)[i] == '(')
				{
					std::cout << "прочитал скобку, итерация " << i << std::endl;
					if (editingTree->info == nullptr)
					{
						if (!(isStart || lastIsClose))
						{
							if (!lastIsClose)
							{
								info = new String(tempstr);
								std::cout << "Записал в info" << std::endl;
							}
							delete tempstr;
							editingTree->left = new SyntaxTree;
							s1.push(editingTree->left);
						}
						else
							isStart = true;
						std::cout << "пытаюсь создать строку" << std::endl;
						tempstr = new String();
						std::cout << "end '('" << std::endl;
					}
					else
					{
						if (lastIsClose)
						{
							std::cout << "Пушим вправо" << std::endl;
							editingTree->right = new SyntaxTree;
							s1.push(editingTree->right);
						}
						else
						{
							std::cout << "Пушим влево" << std::endl;
							editingTree->left = new SyntaxTree;
							s1.push(editingTree->left);
						}
					}
					lastIsClose = false;
				}
				else if ((*readedstr)[i] == ')')
				{
					std::cout << "in ')'" << tempstr << std::endl;
					if (tempstr != nullptr)
					{
						if (!tempstr->isEmpty())
						{
							info = new String(tempstr);
							std::cout << "Записал в info" << std::endl;
							delete tempstr;
							tempstr = new String();
						}
					}
					else
						tempstr = new String();
					lastIsClose = true;
					s1.pop();
				}
				else
				{
					tempstr->addSym((*readedstr)[i]);
					lastIsClose = false;
				}
			}
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