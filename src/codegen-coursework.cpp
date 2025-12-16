#include "codegen-coursework.h"
#include <locale>

String progName;
RegState regs;
DynArray<VarElement*>* varList = new DynArray<VarElement*>;
//DynArray constList;
String assemblerProgram;

bool readVars(SyntaxTree* subTree)
{
	SyntaxTree* treeVar = subTree;
	String* addingVarName = nullptr;
	String* addingVarType = nullptr;
	do
	{
		if (treeVar->left != nullptr)
			if (*(treeVar->left->name) == "IdentifierList")
			{
				std::cout << "Прочитал IdentifierList" << std::endl;
				if (treeVar->left->left != nullptr)
				{
					if (*(treeVar->left->left->name) == "Identifier")
						addingVarName = treeVar->left->left->value;
					else
						return false;
				}
				else
					return false;
				if (treeVar->left->right != nullptr)
				{
					if (*(treeVar->left->right->name) == "Type")
						addingVarType = treeVar->left->right->value;
					else
						return false;
				}
				else
					return false;
				varList->push_back(new VarElement(addingVarName, addingVarType));
			}
			else
				return false;
		else
			return false;
		treeVar = treeVar->right;
	} while (treeVar != nullptr);
	return true;
}

const char prgrm[7] = { 'p', 'r', 'o', 'g', 'r', 'a', 'm' };

bool treeToCode(SyntaxTree* treeHead)
{
	SyntaxTree* currentNode = treeHead;
	std::cout << "Начинаю обработку '" << *(currentNode->name) << "'" << std::endl;
	if (*(currentNode->name) != prgrm)
		return false;
	if (currentNode->left == nullptr)
		return false;
	currentNode = currentNode->left;
	if (*(currentNode->name) != "Identifier")
		return false;
	progName = *(currentNode->value);
	if (currentNode->left == nullptr)
		return false;
	currentNode = currentNode->left;
	while (currentNode != nullptr)
	{
		if (*(currentNode->name) == "VarDeclaration")
			if (!readVars(currentNode->left))
				return false;
		currentNode = currentNode->right;
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "D:\\syntaxis\\syntax\\syntax_tree.txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	if (tree1->size() != 0)
	{
		std::cout << "Дерево успешно прочитано.\nРазмер дерева: " << tree1->size() << std::endl;
		std::cout << "Прочитанное дерево:\n" << *tree1 << std::endl;
		std::cout << "\nЗапуск перевода дерева в код" << std::endl;
		if (treeToCode(tree1))
		{
			std::cout << "Дерево успешно транслировано" << std::endl;
			std::cout << "Размер дерева: " << varList->size() << std::endl;
			for (int i = 0; i < varList->size(); i++)
			{
				std::cout << *(*varList)[i]->name << " " << *(*varList)[i]->type << std::endl;
			}
		}
		else
			std::cout << "Ошибка трансляции дерева" << std::endl;
	}
	else
		std::cout << "Ошибка чтения дерева." << std::endl;
	//td::cout << regs.returnRegName(10) << std::endl;
	return 0;
}
