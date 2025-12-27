#include "codegen-coursework.h"
#include <locale>

String* progName = nullptr;
RegState regs;
DynArray<VarElement*>* varList = new DynArray<VarElement*>;
DynArray<ConstElement*>* constList = new DynArray<ConstElement*>;
DynArray<Function*>* functionList = new DynArray<Function*>;
String* assemblerConsts = new String();
String* assemblerData = new String();
String* assemblerSequence = new String();
String* assemblerFunctions = new String();
String* assemblerProgram = new String();
bool useWriteln = false;
int intSize = 2;

bool readVars(SyntaxTree* subTree, Function* funcToAdd)
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
		if (funcToAdd == nullptr)
			varList->push_back(new VarElement(addingVarName, addingVarType));
		else
		{
			funcToAdd->addVar(new VarElement(addingVarName, addingVarType));
		}
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
	StatElement* tempElem = new StatElement(subTree->name, subTree->value);
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

bool genAssigment(SyntaxTree* assignHead, Function* operatingFunction, String* addingSequence)
{
	String* varName = nullptr;
	bool usePointer = false;
	if (assignHead->left != nullptr)
	{
		varName = assignHead->left->value;
		if (operatingFunction != nullptr)
		{
			int varNum = operatingFunction->returnVarPlace(assignHead->left->value);
			int paramNum = operatingFunction->returnParamPlace(assignHead->left->value);
			bool varParam = false;
			if (varNum != 0)
			{
				varName->addMultiChar("[RBP+");
				varNum *= 8;
				varName->addMultiChar((char*)varNum);
				varName->addMultiChar("]");
				varParam = true;
			}
			if (paramNum != 0)
			{
				ParamElement* elemForAssign = operatingFunction->returnParam(varName);
				if (*elemForAssign->elemType == 1)
				{
					if (varParam)
						delete varName;
					varName = new String();
					varName->addMultiChar("[RBP-");
					varNum *= 8;
					varName->addMultiChar((char*)paramNum);
					varName->addMultiChar("]");
				}
				if (*elemForAssign->elemType == 2)
				{
					if (varParam)
						delete varName;
					varName = new String();
					varNum *= 8;
					varName->addMultiChar((char*)paramNum);
					usePointer = true;
				}
			}
		}
	}
	else
		return false;
	if (assignHead->right != nullptr)
	{
		if ((*assignHead->right->name == "DECNUM") || (*assignHead->right->name == "HEXNUM"))
		{
			addingSequence->addMultiChar("mov EAX, ");
			addingSequence->addString(assignHead->right->value);
			if (!usePointer)
			{
				addingSequence->addMultiChar("\nmov ");
				addingSequence->addString(varName);
				addingSequence->addMultiChar(", EAX\n");
			}
			else
			{
				addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
				addingSequence->addString(varName);
				addingSequence->addMultiChar("\nmov [RBX], EAX\n");
			}
		}
		if (*assignHead->right->name == "BIN_OP")
		{
			Stack<StatElement*>* reversePolNot = new Stack<StatElement*>();
			readRPN(assignHead->right, reversePolNot);
			int stackSize = reversePolNot->size();
			for (int i = 0; i < stackSize; i++)
			{
				if (*reversePolNot->top()->type == "ID")
				{
					addingSequence->addMultiChar("mov EAX, ");
					addingSequence->addString(reversePolNot->top()->value);
					addingSequence->addMultiChar("\npush RAX\n");
				}
				if ((*reversePolNot->top()->type == "DECNUM") || (*reversePolNot->top()->type == "HEXNUM"))
				{
					addingSequence->addMultiChar("mov EAX, ");
					addingSequence->addString(reversePolNot->top()->value);
					addingSequence->addMultiChar("\npush RAX\n");
				}
				if (*reversePolNot->top()->type == "BIN_OP")
				{
					addingSequence->addMultiChar("pop RBX\n");
					addingSequence->addMultiChar("pop RAX\n");
					if (*reversePolNot->top()->value == "+")
						addingSequence->addMultiChar("add EAX, EBX\n");
					else if (*reversePolNot->top()->value == "-")
						addingSequence->addMultiChar("sub EAX, EBX\n");
					else if (*reversePolNot->top()->value == "*")
						addingSequence->addMultiChar("imul EAX, EBX\n");
					else if (*reversePolNot->top()->value == "/")
					{
						addingSequence->addMultiChar("xor EDX, EDX\n");
						addingSequence->addMultiChar("idiv EBX\n");
					}
					addingSequence->addMultiChar("push RAX\n");
				}
				reversePolNot->pop();
			}
			if (!usePointer)
			{
				addingSequence->addMultiChar("pop RAX\n");
				addingSequence->addMultiChar("mov ");
				addingSequence->addString(varName);
				addingSequence->addMultiChar(", EAX");
				addingSequence->addMultiChar("\n");
			}
			else
			{
				addingSequence->addMultiChar("pop RAX\n");
				addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
				addingSequence->addString(varName);
				addingSequence->addMultiChar("\nmov [RBX], EAX\n");
			}
		}
	}
	else
		return false;
	return true;
}

bool readFuncParams(SyntaxTree* paramHead, Function* funcToAdd)
{
	int elemType = 0;
	String* paramName = nullptr;
	String* paramType = nullptr;
	if (*paramHead->name == "PARAM_VAL")
		elemType = 1;
	else if (*paramHead->name == "PARAM_VAR")
		elemType = 2;
	else if (*paramHead->name == "PARAM_CONST")
		elemType = 3;
	else
		return false;
	if (paramHead->left != nullptr)
		if (*(paramHead->left)->name == "ID")
			paramName = paramHead->left->value;
		else
			return false;
	else
		return false;
	if (paramHead->right != nullptr)
		if (*(paramHead->right)->name == "TYPE")
			paramType = paramHead->right->value;
		else
			return false;
	else
		return false;
	std::cout << "Хочу создать элемент параметров" << std::endl;
	ParamElement* paramToWrite = new ParamElement(paramName, paramType, elemType);
	funcToAdd->addParameter(paramToWrite);
	return true;
}

bool genFuncDecl(SyntaxTree* funcHead)
{
	String* funcName = nullptr;
	if (funcHead->left != nullptr)
	{
		funcName = funcHead->left->value;
	}
	else
		return false;

	Function* newFunction = new Function(funcName);

	if (funcHead->right != nullptr)
	{
		SyntaxTree* splitPlace = funcHead->right;
		SyntaxTree* currentNode = splitPlace->left;
		bool notEnd = true;
		std::cout << "Начинаю обход параметров" << std::endl;
		if (currentNode != nullptr)
		{
			while (currentNode != nullptr && notEnd)
			{
				if (*currentNode->name == "SEQ")
				{
					if (currentNode->left != nullptr)
					{
						std::cout << "Читаю параметры начала и середины" << std::endl;
						if (!readFuncParams(currentNode->left, newFunction))
							return false;
					}
					else
						return false;
				}
				else
				{
					std::cout << "Читаю конечный параметр. " << std::endl; //*currentNode->left->name << std::endl;
					notEnd = false;
					if (!readFuncParams(currentNode, newFunction))
					{
						return false;
					}
				}
				currentNode = currentNode->right;
			}

			newFunction->printParams();

			currentNode = splitPlace->right;
			//if (currentNode != nullptr)
			//{
			//	while (currentNode != nullptr && notEnd)
			//	{
			//		if (*currentNode->name == "SEQ")
			//		{
			//			if (currentNode->left != nullptr)
			//			{
			//				std::cout << "Читаю использование начала и середины" << std::endl;
			//				if (!readFuncParams(currentNode->left, paramList))
			//					return false;
			//			}
			//			else
			//				return false;
			//		}
			//		else
			//		{
			//			std::cout << "Читаю конечное использование. " << std::endl; //*currentNode->left->name << std::endl;
			//			notEnd = false;
			//			if (!readFuncParams(currentNode, paramList))
			//			{
			//				return false;
			//			}
			//		}
			//		currentNode = currentNode->right;
			//	}
			//}
		}
		else
			return false;
	}
	else
		return false;
	return true;
}

bool genWritelnCall(SyntaxTree* funcHead, Function* useInFunc, String* addingSequence)
{
	useWriteln = true;
	if (useInFunc == nullptr)
	{
		assemblerSequence->addMultiChar("mov EAX, ");
		if (funcHead->left != nullptr)
		{
			assemblerSequence->addString(funcHead->left->value);
		}
		else
			return false;
	}
	else
	{
		bool paramUsed = false;
		bool varUsed = false;
		int paramNum = useInFunc->returnParamPlace(funcHead->left->value);
		int varNum = useInFunc->returnVarPlace(funcHead->left->value);
		if (paramNum != 0)
		{
			ParamElement* requiredParam = useInFunc->returnParam(funcHead->left->value);
			if ((*requiredParam->elemType == 2) || (*requiredParam->elemType == 3))
			{
				addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
				paramNum *= 8;
				addingSequence->addMultiChar((char*)paramNum);
				addingSequence->addMultiChar("\nmov EAX, [RBX]\n");
				paramUsed = true;
			}
			else if (*requiredParam->elemType == 1)
			{
				addingSequence->addMultiChar("mov EAX, [RBP+");
				paramNum *= 8;
				addingSequence->addMultiChar((char*)paramNum);
				addingSequence->addMultiChar("]\n");
				paramUsed = true;
			}
		}
		if ((varNum != 0) && (!paramUsed))
		{
			varNum *= 8;
			addingSequence->addMultiChar("mov EAX, [RBP-");
			addingSequence->addMultiChar((char*)varNum);
			addingSequence->addMultiChar("]\n");
			varUsed = true;
		}
		if ((!paramUsed) && (!varUsed))
			assemblerSequence->addString(funcHead->left->value);
	}
	assemblerSequence->addMultiChar("\npush RAX");
	assemblerSequence->addMultiChar("\ncall writeln\n");
	return true;
}

bool genVars()
{
	for (int i = 0; i < varList->size(); i++)
	{
		String* varType = (*varList)[i]->type;
		String varSize;
		if (*varType == "INTEGER")
			varSize.addMultiChar("sdword");
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

bool processDecls(SyntaxTree* currentNode)
{
	if (*(currentNode->name) == "VAR_DECL")
		if (!readVars(currentNode, nullptr))
			return false;
	if (*(currentNode->name) == "CONST_DECL")
		if (!readConsts(currentNode))
			return false;
	if (*(currentNode->name) == "FUNCTION")
		if (!genFuncDecl(currentNode))
			return false;
	return true;
}

bool processUsing(SyntaxTree* currentNode, Function* inFunc, String* usingSequence)
{
	if (*(currentNode->name) == "ASSIGN")
		if (!genAssigment(currentNode, inFunc, usingSequence))
			return false;
	//if (*(currentNode->name) == "FUNC_CALL")
	//	if (!genFuncCall(currentNode))
	//		return false;
	if (*(currentNode->name) == "WRITELN")
		if (!genWritelnCall(currentNode, inFunc, usingSequence))
			return false;
	return true;
}

bool parseTree(SyntaxTree* treeHead)
{
	SyntaxTree* currentNode = treeHead;
	if (*(currentNode->name) != "PROGRAM")
		return false;
	if (currentNode->left == nullptr)
	{
		progName = new String();
		progName->addMultiChar("result");
	}
	else
		if (*(currentNode->left->name) == "ID")
			progName = currentNode->left->value;
		else
			return false;
	currentNode = currentNode->right;
	SyntaxTree* splitPlace = currentNode;
	if (currentNode != nullptr)
	{
		currentNode = currentNode->left;
		while (currentNode != nullptr)
		{
			if (*currentNode->name == "SEQ")
			{
				if (currentNode->left != nullptr)
				{
					if (!processDecls(currentNode->left))
						return false;

				}
				else
					return false;
			}
			else
				if (!processDecls(currentNode))
					return false;
			currentNode = currentNode->right;
		}
		currentNode = splitPlace->right;
		while (currentNode != nullptr)
		{
			if (*currentNode->name == "SEQ")
			{
				if (currentNode->left != nullptr)
				{
					if (!processUsing(currentNode->left, nullptr, assemblerSequence))
						return false;

				}
				else
					return false;
			}
			else
				if (!processUsing(currentNode, nullptr, assemblerSequence))
					return false;
			currentNode = currentNode->right;
		}
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "C:\\Users\\MaxStep\\source\\repos\\syntax3\\syntax\\syntax_tree.txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	if (tree1->size() != 0)
	{
		std::cout << "Дерево успешно прочитано.\nРазмер дерева: " << tree1->size() << std::endl;
		std::cout << "Прочитанное дерево:\n" << *tree1 << std::endl;
		std::cout << "\nЗапуск перевода дерева в код" << std::endl;
		if (parseTree(tree1))
		{
			std::cout << "Дерево успешно транслировано" << std::endl;
			std::cout << "Имя программы: " << *progName << std::endl;
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
			if (useWriteln)
				assemblerProgram->addMultiChar("includelib kernel32.lib\nextrn WriteFile : PROC\nextrn GetStdHandle : PROC\n\n");
			assemblerProgram->addString(assemblerConsts);
			assemblerProgram->addMultiChar("\n");
			assemblerProgram->addMultiChar(".data\n");
			genVars();
			assemblerProgram->addString(assemblerData);
			if (useWriteln)
			{
				assemblerProgram->addMultiChar("\noutSyms word 0\n"
					"bytesWritten word 0\n\n"
					".code\n"
					"printSym proc\n"
					"sub RSP, 40\n"
					"mov RCX, -11\n"
					"call GetStdHandle\n"
					"mov RCX, RAX\n"
					"lea RDX, [RSP + 48]\n"
					"mov r8d, 1\n"
					"lea r9, bytesWritten\n"
					"mov qword ptr[rsp + 32], 0\n"
					"call WriteFile\n"
					"add RSP, 40\n"
					"ret\n"
					"printSym endp\n\n"
					"writeln proc\n"
					"pop R15\n"
					"pop RAX\n"
					"cmp EAX, 0\n"
					"jns m1\n"
					"mov R14D, EAX\n"
					"push '-'\n"
					"inc outSyms\n"
					"call printSym\n"
					"add RSP, 2\n"
					"mov EAX, R14D\n"
					"neg EAX\n"
					"m1: xor EDX, EDX\n"
					"mov EBX, 10\n"
					"div EBX\n"
					"add EDX, 48\n"
					"push DX\n"
					"inc outSyms\n"
					"cmp EAX, 0\n"
					"jnz m1\n"
					"m2: call printSym\n"
					"add RSP, 2\n"
					"dec outSyms\n"
					"cmp outSyms, 0\n"
					"jnz m2\n"
					"push 10\n"
					"call printSym\n"
					"add RSP, 2\n"
					"push R15\n"
					"ret\n"
					"writeln endp\n\n"
					"main proc\n"
				);
			}
			else
				assemblerProgram->addMultiChar("\n.code\nmain proc\n");
			assemblerProgram->addString(assemblerSequence);
			assemblerProgram->addMultiChar("main endp\nend\n");
			std::cout << "\nРезультирующая программа:\n" << *assemblerProgram << std::endl;
			String fileName;
			fileName.addString(progName);
			fileName.addMultiChar(".asm");
			assemblerProgram->writeToFile(fileName.toChar());
		}
		else
			std::cout << "Ошибка трансляции дерева" << std::endl;
	}
	else
		std::cout << "Ошибка чтения дерева." << std::endl;
	//td::cout << regs.returnRegName(10) << std::endl;
	return 0;
}
