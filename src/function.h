#pragma once
#include "paramelement.h"
#include "varelement.h"
#include "string.h"
#include "dynarray.h"

class Function
{
private:
	String* name = nullptr;
	String* functionSequence = nullptr;
	DynArray<ParamElement*>* parameterList = nullptr;
	int localVarCount = 1; // первая - результат
	DynArray<VarElement*>* localVarList = nullptr;
	String* funcType = nullptr;
public:
	Function(String* inpName)
	{
		name = new String(inpName);
		functionSequence = new String();
		functionSequence->addMultiChar("pop R12\nmov R13, RBP\nmov RBP, RSP\n");
	}
	Function(String* inpName, DynArray<ParamElement*>* inpParameterList)
	{
		name = new String(inpName);
		parameterList = new DynArray<ParamElement*>(*inpParameterList);
		functionSequence = new String();
		functionSequence->addMultiChar("pop R12\nmov R13, RBP\nmov RBP, RSP\n");
		//int size = 
		//for (int i)
	}
	~Function()
	{
		delete name;
		if (parameterList != nullptr)
			delete parameterList;
		delete functionSequence;
	}

	void addParameter(ParamElement* paramToAdd)
	{
		if (parameterList == nullptr)
			parameterList = new DynArray<ParamElement*>();
		parameterList->push_back(paramToAdd);
	}

	void printParams()
	{
		int size = parameterList->size();
		for (int i = 0; i < size; i++)
		{
			std::cout << *(*parameterList)[i]->name << " " << *(*parameterList)[i]->dataType << " " << *(*parameterList)[i]->elemType << std::endl;
		}
	}

	void setFuncType(String* type)
	{
		if (funcType != nullptr)
			delete funcType;
		funcType = type;
	}

	ParamElement* returnParam(String* param) 
	{
		if (parameterList != nullptr)
		{
			int size = parameterList->size();
			for (int i = 0; i < size; i++)
			{
				if (*(*parameterList)[i]->name == *param)
					return (*parameterList)[i];
			}
		}
		return nullptr;
	}

	int returnParamPlace(String* param)
	{
		if (parameterList != nullptr)
		{
			int size = parameterList->size();
			for (int i = 0; i < size; i++)
			{
				std::cout << "Проверяю соответствие запрашиваемого '" << *param << "' имеющемуся '" << *(*parameterList)[i]->name << "'" << std::endl;
				if (*(*parameterList)[i]->name == *param)
				{
					std::cout << "Соответствие найдено" << std::endl;
					return i + 1;
				}
			}
		}
		std::cout << "Соответствие не найдено" << std::endl;
		return 0;
	}

	int returnVarPlace(String* var)
	{
		if (localVarList != nullptr)
		{
			int size = parameterList->size();
			for (int i = 0; i < size; i++)
			{
				if (*(*localVarList)[i]->name == *var)
					return i + 1;
			}
		}
		return 0;
	}

	String* returnFuncSeq()
	{
		return functionSequence;
	}

	//DynArray<VarElement*>* returnLocalVarList()
	//{
	//	return localVarList;
	//}
	void addVar(VarElement* addingVar)
	{
		if (localVarList == nullptr)
			localVarList = new DynArray<VarElement*>();
		localVarList->push_back(addingVar);
		localVarCount++;
	}

	//int returnLocalVarSpace()
	//{
	//	return localVarCount * 8; // пока тип только integer
	//}

	void genLocalVarSpace()
	{
		int space = localVarCount * 8; // тип только integer
		char buff[6];
		sprintf(buff, "%d\n", space);
		functionSequence->addMultiChar("sub RSP, ");
		functionSequence->addMultiChar(buff);
	}

	void genSeqEnd()
	{
		functionSequence->addMultiChar("mov R11, [RBP]\n");
		int space = (localVarCount + parameterList->size()) * 8; // пока тип только integer
		char buff[6];
		sprintf(buff, "%d\n", space);
		functionSequence->addMultiChar("add RSP, ");
		functionSequence->addMultiChar(buff);
		functionSequence->addMultiChar("mov RBP, R13\npush R11\npush R12\nret\n");
	}

	String* returnFuncName()
	{
		return name;
	}

	int returnParamType(int natPlace)
	{
		if (natPlace <= parameterList->size())
			return *(*parameterList)[natPlace-1]->elemType;
		else
			return 0;
	}
};