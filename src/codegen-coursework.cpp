#include "codegen-coursework.h"
#include <locale>

String progName;
RegState regs;
DynArray<VarElement*>* varList = new DynArray<VarElement*>;
DynArray <ConstElement*>* constList = new DynArray<ConstElement*>;
String* assemblerConsts = new String();
String* assemblerData = new String();
String* assemblerSequence = new String();
String* assemblerProgram = new String();

bool readVars(SyntaxTree* subTree)
{
	SyntaxTree* treeVar = subTree;
	String* addingVarName = nullptr;
	String* addingVarType = nullptr;
		if (*treeVar->name == "VAR_DECL")
		{
					if (treeVar->left != nullptr)
					{
						if (*(treeVar->left->name) == "ID")
							addingVarName = treeVar->left->value;
						else
							return false;
					}
					else
						return false;
					if (treeVar->right != nullptr)
					{
						if (*(treeVar->right->name) == "TYPE")
							addingVarType = treeVar->right->value;
						else
							return false;
					}
					else
						return false;
					varList->push_back(new VarElement(addingVarName, addingVarType));
		}
		else
			return false;
	return true;
}

bool readConsts(SyntaxTree* subTree)
{
	SyntaxTree* treeVar = subTree;
	String* addingConstName = nullptr;
	String* addingConstType = nullptr;
	String* addingConstValue = nullptr;
		if (*treeVar->name == "CONST_DECL")
		{
			if (treeVar->left != nullptr)
			{
				if (*(treeVar->left->name) == "ID")
					addingConstName = treeVar->left->value;
				else
					return false;
			}
			else
				return false;
			if (treeVar->right != nullptr)
			{
				addingConstType = treeVar->right->name;
				addingConstValue = treeVar->right->value;

			}
			else
				return false;
			constList->push_back(new ConstElement(addingConstName, addingConstType, addingConstValue));
		}
		else
			return false;
	return true;
}

void readRPN(SyntaxTree* subTree, Stack<StatElement*>* storage)
{
	//std::cout << "Меня вызвали" << std::endl;
	StatElement* tempElem = new StatElement(subTree->name, subTree->value);
	std::cout << "Пишу:" << std::endl;
	std::cout << *tempElem->type << ":" << *tempElem->value << std::endl;
	storage->push(tempElem);
	if (subTree->right != nullptr)
	{
		readRPN(subTree->right, storage);
	}
	if (subTree->left != nullptr)
	{
		readRPN(subTree->left, storage);
	}
}

bool genAssigment(SyntaxTree* assignHead)
{
	std::cout << "Опоп, присвоение" << std::endl;
	String* varName = nullptr;
	if (assignHead->left != nullptr)
		varName = assignHead->left->value;
	else
		return false;
	if (assignHead->right != nullptr)
	{
		if (*assignHead->right->name == "DECNUM")
		{
			assemblerSequence->addMultiChar("mov AX, ");
			assemblerSequence->addString(assignHead->right->value);
			assemblerSequence->addMultiChar("\nmov ");
			assemblerSequence->addString(varName);
			assemblerSequence->addMultiChar(", AX\n");
		}
		if (*assignHead->right->name == "HEXNUM")
		{
			assemblerSequence->addMultiChar("mov AX, 0x");
			assemblerSequence->addString(assignHead->right->value);
			assemblerSequence->addMultiChar("\nmov ");
			assemblerSequence->addString(varName);
			assemblerSequence->addMultiChar(", AX\n");
		}
		if (*assignHead->right->name == "BIN_OP")
		{
			Stack<StatElement*>* reversePolNot = new Stack<StatElement*>();
			readRPN(assignHead->right, reversePolNot);
			std::cout << "Выводим. Размер стека: " << reversePolNot->size() << std::endl;
			int stackSize = reversePolNot->size();
			for (int i = 0; i < stackSize; i++)
			{
				if (*reversePolNot->top()->type == "ID")
				{
					assemblerSequence->addMultiChar("mov AX, ");
					assemblerSequence->addString(reversePolNot->top()->value);
					assemblerSequence->addMultiChar("\npush AX\n");
				}
				if ((*reversePolNot->top()->type == "DECNUM") || (*reversePolNot->top()->type == "HEXNUM"))
				{
					assemblerSequence->addMultiChar("push ");
					assemblerSequence->addString(reversePolNot->top()->value);
					assemblerSequence->addMultiChar("\n");
				}
				if (*reversePolNot->top()->type == "BIN_OP")
				{
					assemblerSequence->addMultiChar("pop BX\n");
					assemblerSequence->addMultiChar("pop AX\n");
					if (*reversePolNot->top()->value == "+")
						assemblerSequence->addMultiChar("add AX, BX\n");
					else if (*reversePolNot->top()->value == "-")
						assemblerSequence->addMultiChar("sub AX, BX\n");
					else if (*reversePolNot->top()->value == "*")
						assemblerSequence->addMultiChar("imul AX, BX\n");
					else if (*reversePolNot->top()->value == "/")
					{
						assemblerSequence->addMultiChar("isub AX\n");
						assemblerSequence->addMultiChar("mov AH, 0\n");
					}
					assemblerSequence->addMultiChar("push AX\n");
				}
				//std::cout << *reversePolNot->top()->type << " " << *reversePolNot->top()->value << std::endl;
				reversePolNot->pop();
			}
			assemblerSequence->addMultiChar("pop ");
			assemblerSequence->addString(varName);
			assemblerSequence->addMultiChar("\n");
		}
	}
	else
		return false;
	return true;
}

bool genVars()
{
	for (int i = 0; i < varList->size(); i++)
	{
		String* varType = (*varList)[i]->type;
		String varSize;
		if (*varType == "INTEGER")
			varSize.addMultiChar("sqword");
		else
			return false;
		assemblerData->addString((*varList)[i]->name);
		assemblerData->addMultiChar(" ");
		assemblerData->addString(&varSize);
		assemblerData->addMultiChar(" 0\n");
	}
	return true;
}

bool genConsts()
{
	for (int i = 0; i < constList->size(); i++)
	{
		assemblerConsts->addString((*constList)[i]->name);
		assemblerConsts->addMultiChar(" equ ");
		if ((!(*(*constList)[i]->type == "DECNUM")) && (!(*(*constList)[i]->type == "HEXNUM")))
			return false;
		assemblerConsts->addString((*constList)[i]->value);
		assemblerConsts->addMultiChar("\n");
	}
	return true;
}

bool parseTree(SyntaxTree* treeHead)
{
	SyntaxTree* currentNode = treeHead;
	std::cout << "Начинаю обработку '" << *(currentNode->name) << "'" << std::endl;
	if (*(currentNode->name) != "PROGRAM")
		return false;
	if (currentNode->left == nullptr)
		return false;
	if (*(currentNode->left->name) != "ID")
		return false;
	progName = *(currentNode->value);
	currentNode = currentNode->right;
	while (currentNode != nullptr)
	{
		if (currentNode->left != nullptr)
		{
			if (*(currentNode->left->name) == "VAR_DECL")
				if (!readVars(currentNode->left))
					return false;
			if (*(currentNode->left->name) == "CONST_DECL")
				if (!readConsts(currentNode->left))
					return false;
			if (*(currentNode->left->name) == "ASSIGN")
				if (!genAssigment(currentNode->left))
					return false;
			currentNode = currentNode->right;
		}
		else
			return false;
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "D:\\naturaltree.txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	if (tree1->size() != 0)
	{
		std::cout << "Дерево успешно прочитано.\nРазмер дерева: " << tree1->size() << std::endl;
		std::cout << "Прочитанное дерево:\n" << *tree1 << std::endl;
		std::cout << "\nЗапуск перевода дерева в код" << std::endl;
		if (parseTree(tree1))
		{
			std::cout << "Дерево успешно транслировано" << std::endl;
			std::cout << "Размер массива переменных: " << varList->size() << std::endl;
			for (int i = 0; i < varList->size(); i++)
			{
				std::cout << *(*varList)[i]->name << " " << *(*varList)[i]->type << std::endl;
			}
			std::cout << "\nРазмер массива констант: " << constList->size() << std::endl;
			for (int i = 0; i < constList->size(); i++)
			{
				std::cout << *(*constList)[i]->name << " " << *(*constList)[i]->type << " " << *(*constList)[i]->value << std::endl;
			}
			genConsts();
			assemblerProgram->addString(assemblerConsts);
			assemblerProgram->addMultiChar("\n");
			assemblerProgram->addMultiChar(".data\n");
			genVars();
			assemblerProgram->addString(assemblerData);
			assemblerProgram->addMultiChar("\n.code\nmain proc\n");
			assemblerProgram->addString(assemblerSequence);
			assemblerProgram->addMultiChar("main endp\nend.\n");
			std::cout << "\nРезультирующая программа:\n" << *assemblerProgram << std::endl;
		}
		else
			std::cout << "Ошибка трансляции дерева" << std::endl;
	}
	else
		std::cout << "Ошибка чтения дерева." << std::endl;
	//td::cout << regs.returnRegName(10) << std::endl;
	return 0;
}
