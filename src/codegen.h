#pragma once

#include <iostream>
#include <locale>

#include "string.h"
#include "binarytree.h"
#include "stack.h"
#include "syntaxtree.h"
#include "varelement.h"
#include "constelement.h"
#include "statelement.h"
#include "paramelement.h"
#include "function.h"

class Codegen
{
private:
	String* progName = nullptr;
	DynArray<VarElement*>* varList = nullptr;
	DynArray<ConstElement*>* constList = nullptr;
	DynArray<Function*>* functionList = nullptr;
	String* assemblerConsts = nullptr;
	String* assemblerData = nullptr;
	String* assemblerSequence = nullptr;
	String* assemblerFunctions = nullptr;
	String* assemblerProgram = nullptr;
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

	void readRPN(SyntaxTree* subTree, Stack<StatElement*>* storage, Function* usingFunction)
	{
		bool pointerParamUse = false;
		String* valueToAdd = subTree->value;
		if (usingFunction != nullptr)
		{
			bool paramUsed = false;
			int paramNum = usingFunction->returnParamPlace(valueToAdd);
			int varNum = usingFunction->returnVarPlace(valueToAdd);
			if (paramNum != 0)
			{
				ParamElement* requiredParam = usingFunction->returnParam(valueToAdd);
				if ((*requiredParam->elemType == 2) || (*requiredParam->elemType == 3))
				{
					char buff[6];
					paramNum *= 8;
					valueToAdd = new String();
					sprintf(buff, "%d", paramNum);
					valueToAdd->addMultiChar(buff);
					pointerParamUse = true;
					paramUsed = true;
				}
				else if (*requiredParam->elemType == 1)
				{
					char buff[6];
					valueToAdd = new String();
					valueToAdd->addMultiChar("[RBP+");
					paramNum *= 8;
					sprintf(buff, "%d", paramNum);
					valueToAdd->addMultiChar(buff);
					valueToAdd->addMultiChar("]");
					paramUsed = true;
				}
			}
			if ((varNum != 0) && (!paramUsed))
			{
				char buff[6];
				varNum *= 8;
				valueToAdd = new String();
				valueToAdd->addMultiChar("[RBP-");
				sprintf(buff, "%d", varNum);
				valueToAdd->addMultiChar(buff);
				valueToAdd->addMultiChar("]");
			}
		}
		StatElement* tempElem = new StatElement(subTree->name, valueToAdd, pointerParamUse);
		storage->push(tempElem);
		if (subTree->right != nullptr)
		{
			readRPN(subTree->right, storage, usingFunction);
		}
		if (subTree->left != nullptr)
		{
			readRPN(subTree->left, storage, usingFunction);
		}
	}

	bool readFuncCallParam(SyntaxTree* paramHead, Function* callingFunction, Stack<ParamElement*>* parameterStack, int paramNum)
	{
		String* name = nullptr;
		String* dataType = nullptr;
		int elemType = 0;
		if (paramHead->left != nullptr)
		{
			if (*paramHead->left->name == "ID")
			{
				name = paramHead->left->value;
			}
			else
				return false;
		}
		else
			return false;
		if (paramHead->right != nullptr)
		{
			if (*paramHead->right->name == "TYPE")
			{
				dataType = paramHead->left->value;
			}
			else
				return false;
		}
		else
			return false;
		elemType = callingFunction->returnParamType(paramNum);
		ParamElement* newCallParam = new ParamElement(name, dataType, elemType);
		//delete name;
		//delete dataType;
		parameterStack->push(newCallParam);
		return true;
	}

	bool genFuncCall(SyntaxTree* callHead, Function* operatingFunction, String* addingSequence)
	{
		if (callHead != nullptr)
		{
			if (*callHead->left->name == "ID")
			{
				int paramNum = 1;
				String* funcName = callHead->left->value;
				int funcListSize = functionList->size();
				Function* callingFunction = nullptr;
				for (int i = 0; i < funcListSize; i++)
				{
					if (*(*functionList)[i]->returnFuncName() == *funcName)
						callingFunction = (*functionList)[i];
				}
				if (callingFunction == nullptr)
					return false;
				SyntaxTree* currentNode = callHead->right;
				Stack<ParamElement*>* parameterStack = new Stack<ParamElement*>;
				bool notEnd = true;
				if (currentNode != nullptr)
				{
					while (currentNode != nullptr && notEnd)
					{
						if (*currentNode->name == "SEQ")
						{
							if (currentNode->left != nullptr)
							{
								if (!readFuncCallParam(currentNode->left, callingFunction, parameterStack, paramNum))
									return false;
							}
							else
								return false;
						}
						else
						{
							notEnd = false;
							if (!readFuncCallParam(currentNode, callingFunction, parameterStack, paramNum))
							{
								return false;
							}
						}
						currentNode = currentNode->right;
						paramNum++;
					}
				}
				ParamElement* currElem = nullptr;
				while (parameterStack->size() != 0)
				{
					currElem = parameterStack->top();
					if (*currElem->elemType == 1)
					{
						addingSequence->addMultiChar("mov EAX, ");
						addingSequence->addString(currElem->name);
						addingSequence->addMultiChar("\npush RAX\n");
					}
					else if ((*currElem->elemType == 2) || (*currElem->elemType = 3))
					{
						addingSequence->addMultiChar("lea RAX, ");
						addingSequence->addString(currElem->name);
						addingSequence->addMultiChar("\npush RAX\n");
					}
					else
						return false;
					parameterStack->pop();
					delete currElem;
				}
				addingSequence->addMultiChar("call ");
				addingSequence->addString(funcName);
				addingSequence->addMultiChar("\n");
			}
			else
				return false;
		}
		else
			return false;
		return true;
	}

	bool genAssigment(SyntaxTree* assignHead, Function* operatingFunction, String* addingSequence)
	{
		String* varName = nullptr;
		bool usePointer = false;
		if (assignHead->left != nullptr)
		{
			if (operatingFunction != nullptr)
			{
				if (*assignHead->left->value != *operatingFunction->returnFuncName())
				{
					int varNum = operatingFunction->returnVarPlace(assignHead->left->value);
					int paramNum = operatingFunction->returnParamPlace(assignHead->left->value);
					bool useVar = false;
					bool useParam = false;
					if (varNum != 0)
					{
						varName = new String();
						char buff[6];
						varName->addMultiChar("[RBP-");
						varNum *= 8;
						sprintf(buff, "%d", varNum);
						varName->addMultiChar(buff);
						varName->addMultiChar("]");
						useVar = true;
					}
					if (paramNum != 0)
					{
						ParamElement* elemForAssign = operatingFunction->returnParam(assignHead->left->value);
						if (*elemForAssign->elemType == 1)
						{
							char buf[6];
							if (useVar)
								delete varName;
							varName = new String();
							varName->addMultiChar("[RBP+");
							paramNum *= 8;
							sprintf(buf, "%d", paramNum);
							varName->addMultiChar(buf);
							varName->addMultiChar("]");
							useParam = true;
						}
						if (*elemForAssign->elemType == 2)
						{
							char buf[6];
							if (useVar)
								delete varName;
							varName = new String();
							paramNum *= 8;
							sprintf(buf, "%d", paramNum);
							varName->addMultiChar(buf);
							usePointer = true;
							useParam = true;
						}
					}
					if ((!useVar) && (!useParam))
					{
						varName = assignHead->left->value;
					}
				}
				else
				{
					varName = new String();
					varName->addMultiChar("[RBP]");
				}
			}
			else
				varName = assignHead->left->value;
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
					addingSequence->addMultiChar("\nmov RBX, RBP\nadd RBX, ");
					addingSequence->addString(varName);
					addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov [RBX], EAX\n");
				}
			}
			if (*assignHead->right->name == "ID")
			{
				bool paramUsed = false;
				bool pointerRightUse = false;
				bool varUsed = false;
				int paramNum = operatingFunction->returnParamPlace(assignHead->right->value);
				int varNum = operatingFunction->returnVarPlace(assignHead->right->value);
				String* addrOfRight = nullptr;
				if (paramNum != 0)
				{
					ParamElement* requiredParam = operatingFunction->returnParam(assignHead->right->value);
					if ((*requiredParam->elemType == 2) || (*requiredParam->elemType == 3))
					{
						char buff[6];
						paramNum *= 8;
						sprintf(buff, "%d", paramNum);
						addrOfRight = new String();
						addrOfRight->addMultiChar(buff);
						pointerRightUse = true;
						paramUsed = true;
					}
					else if (*requiredParam->elemType == 1)
					{
						char buff[6];
						addrOfRight = new String();
						addrOfRight->addMultiChar("[RBP+");
						paramNum *= 8;
						sprintf(buff, "%d", paramNum);
						addrOfRight->addMultiChar(buff);
						addrOfRight->addMultiChar("]");
						paramUsed = true;
					}
					if ((varNum != 0) && (!paramUsed))
					{
						char buff[6];
						varNum *= 8;
						addrOfRight = new String();
						addrOfRight->addMultiChar("[RBP-");
						sprintf(buff, "%d", varNum);
						addrOfRight->addMultiChar(buff);
						addrOfRight->addMultiChar("]");
						varUsed = true;
					}
				}
				if (!usePointer)
				{
					if (!pointerRightUse)
					{
						addingSequence->addMultiChar("mov EAX, ");
						if ((!varUsed) && (!paramUsed))
						{
							addingSequence->addString(assignHead->right->value);
							addingSequence->addMultiChar("\n"); //
						}
						else
						{
							addingSequence->addString(addrOfRight);
							addingSequence->addMultiChar("\n"); //
							delete addrOfRight;
						}
					}
					else
					{
						addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
						addingSequence->addString(addrOfRight);
						addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov EAX, [RBX]\n");
					}
					addingSequence->addMultiChar("\nmov ");
					addingSequence->addString(varName);
					addingSequence->addMultiChar(", EAX\n");
				}
				else
				{
					if (!pointerRightUse)
					{
						addingSequence->addMultiChar("\nmov EAX, ");
						if (!varUsed)
						{
							addingSequence->addString(assignHead->right->value);
							addingSequence->addMultiChar("\n"); //
						}
						else
						{
							addingSequence->addString(addrOfRight);
							addingSequence->addMultiChar("\n"); //
							delete addrOfRight;
						}
					}
					else
					{
						addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
						addingSequence->addString(addrOfRight);
						addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov EAX, [RBX]\n");
					}
					addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
					addingSequence->addString(varName);
					addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov [RBX], EAX\n");
				}
			}
			if (*assignHead->right->name == "FUNC_CALL")
			{
				if (!genFuncCall(assignHead->right, operatingFunction, addingSequence))
					return false;
				addingSequence->addMultiChar("push RAX\nmov ");
				addingSequence->addString(varName);
				addingSequence->addMultiChar(", EAX\n");
			}
			if (*assignHead->right->name == "BIN_OP")
			{
				Stack<StatElement*>* reversePolNot = new Stack<StatElement*>();
				readRPN(assignHead->right, reversePolNot, operatingFunction);
				int stackSize = reversePolNot->size();
				for (int i = 0; i < stackSize; i++)
				{
					if (*reversePolNot->top()->type == "ID")
					{
						if (!(reversePolNot->top()->isPointer))
						{
							addingSequence->addMultiChar("mov EAX, ");
							addingSequence->addString(reversePolNot->top()->value);
							addingSequence->addMultiChar("\npush RAX\n");
						}
						else
						{
							addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
							addingSequence->addString(reversePolNot->top()->value);
							addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov EAX, [RBX]\npush RAX\n");
						}
					}
					//if (*reversePolNot->top()->type == "FUNC_CALL")
					//{
					//	genFuncCall()
					//}
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
					addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov [RBX], EAX\n");
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
		ParamElement* paramToWrite = new ParamElement(paramName, paramType, elemType);
		funcToAdd->addParameter(paramToWrite);
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
					char buff[6];
					addingSequence->addMultiChar("mov RBX, RBP\nadd RBX, ");
					paramNum *= 8;
					sprintf(buff, "%d", varNum);
					addingSequence->addMultiChar(buff);
					addingSequence->addMultiChar("\nmov RBX, [RBX]\nmov EAX, [RBX]\n");
					paramUsed = true;
				}
				else if (*requiredParam->elemType == 1)
				{
					char buff[6];
					addingSequence->addMultiChar("mov EAX, [RBP+");
					paramNum *= 8;
					sprintf(buff, "%d", varNum);
					addingSequence->addMultiChar(buff);
					addingSequence->addMultiChar("]\n");
					paramUsed = true;
				}
			}
			if ((varNum != 0) && (!paramUsed))
			{
				char buff[6];
				varNum *= 8;
				addingSequence->addMultiChar("mov EAX, [RBP-");
				sprintf(buff, "%d", varNum);
				addingSequence->addMultiChar(buff);
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

	bool processUsing(SyntaxTree* currentNode, Function* inFunc, String* usingSequence)
	{
		if (*(currentNode->name) == "ASSIGN")
			if (!genAssigment(currentNode, inFunc, usingSequence))
				return false;
		if (*(currentNode->name) == "FUNC_CALL")
		{
			if (!genFuncCall(currentNode, inFunc, usingSequence))
				return false;
			usingSequence->addMultiChar("add RSP, 8\n");
		}
		if (*(currentNode->name) == "WRITELN")
			if (!genWritelnCall(currentNode, inFunc, usingSequence))
				return false;
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
			if (currentNode != nullptr)
			{
				while (currentNode != nullptr && notEnd)
				{
					if (*currentNode->name == "SEQ")
					{
						if (currentNode->left != nullptr)
						{
							if (!readFuncParams(currentNode->left, newFunction))
								return false;
						}
						else
							return false;
					}
					else
					{
						notEnd = false;
						if (!readFuncParams(currentNode, newFunction))
						{
							return false;
						}
					}
					currentNode = currentNode->right;
				}

				currentNode = splitPlace->right;
				if (currentNode != nullptr)
				{
					if (*currentNode->left->name == "type")
						newFunction->setFuncType(currentNode->left->value);
					currentNode = currentNode->right;
					if (currentNode != nullptr)
					{
						if (*currentNode->name == "SEQ")
						{
							splitPlace = currentNode;
							currentNode = currentNode->left;
							notEnd = true;
							while ((currentNode != nullptr) && notEnd)
							{
								if (*currentNode->name == "SEQ")
								{
									if (currentNode->left != nullptr)
									{
										if (!readVars(currentNode->left, newFunction))
											return false;

									}
									else
										return false;
								}
								else
								{
									notEnd = false;
									if (!readVars(currentNode, newFunction))
										return false;
								}
								currentNode = currentNode->right;
							}
							newFunction->genLocalVarSpace();
							currentNode = splitPlace->right;
						}
						if (currentNode != nullptr)
						{
							if (*currentNode->name == "COMPOUND_STMT")
							{
								notEnd = true;
								String* funcSeq = newFunction->returnFuncSeq();
								currentNode = currentNode->left;
								while (currentNode != nullptr && notEnd)
								{
									if (*currentNode->name == "SEQ")
									{
										if (currentNode->left != nullptr)
										{
											if (!processUsing(currentNode->left, newFunction, funcSeq))
												return false;
										}
										else
											return false;
									}
									else
									{
										notEnd = false;
										if (!processUsing(currentNode, newFunction, funcSeq))
										{
											return false;
										}
									}
									currentNode = currentNode->right;
								}
								newFunction->genSeqEnd();
							}
							else
								return false;
						}
					}
				}
			}
			else
				return false;
		}
		else
		{
			return false;
		}
		functionList->push_back(newFunction);
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
public:
	Codegen()
	{
		varList = new DynArray<VarElement*>;
		constList = new DynArray<ConstElement*>;
		functionList = new DynArray<Function*>;
		assemblerConsts = new String();
		assemblerData = new String();
		assemblerSequence = new String();
		assemblerFunctions = new String();
		assemblerProgram = new String();
	}
	void genCode(const char* filename)
	{
		SyntaxTree* tree1 = new SyntaxTree(filename);
		if (tree1->size() != 0)
		{
			std::cout << "Дерево успешно прочитано.\nРазмер дерева: " << tree1->size() << std::endl;
			std::cout << "Прочитанное дерево:\n" << *tree1 << std::endl;
			std::cout << "\nЗапуск перевода дерева в код" << std::endl;
			if (parseTree(tree1))
			{
				std::cout << "Дерево успешно транслировано\n" << std::endl;
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
				std::cout << "\n\nРазмер массива функций: " << functionList->size() << std::endl;
				for (int i = 0; i < constList->size(); i++)
				{
					std::cout << "\nФункция " << *(*functionList)[i]->returnFuncName() << "\nПараметры:" << std::endl;
					(*functionList)[i]->printParams();
				}
				std::cout << "Где 1 - по значению, 2 - var, 3 - const.\n" << std::endl;
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
					);
				}
				else
					assemblerProgram->addMultiChar("\n.code\n");
				int funcCount = functionList->size();
				for (int i = 0; i < funcCount; i++)
				{
					Function* currFunc = (*functionList)[i];
					assemblerProgram->addString(currFunc->returnFuncName());
					assemblerProgram->addMultiChar(" proc\n");
					assemblerProgram->addString(currFunc->returnFuncSeq());
					assemblerProgram->addString(currFunc->returnFuncName());
					assemblerProgram->addMultiChar(" endp\n\n");
				}
				assemblerProgram->addMultiChar("main proc\n");
				assemblerProgram->addString(assemblerSequence);
				assemblerProgram->addMultiChar("ret\nmain endp\nend\n");
				//std::cout << "\nРезультирующая программа:\n" << *assemblerProgram << std::endl;
				String outFilename;
				outFilename.addString(progName);
				outFilename.addMultiChar(".asm");
				assemblerProgram->writeToFile(outFilename.toChar());
				std::cout << "Результат записан в файл '" << outFilename << "'." << std::endl;
			}
			else
				std::cout << "Ошибка трансляции дерева" << std::endl;
		}
		else
			std::cout << "Ошибка чтения дерева." << std::endl;
	}
	~Codegen()
	{
		delete varList;
		delete constList;
		delete functionList;
		delete assemblerConsts;
		delete assemblerData;
		delete assemblerSequence;
		delete assemblerFunctions;
		delete assemblerProgram;
	}
};