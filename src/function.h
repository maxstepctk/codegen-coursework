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
	int localVarCount = 0;
	DynArray<VarElement*>* localVarList = nullptr;
public:
	Function(String* inpName)
	{
		name = new String(inpName);
		functionSequence = new String();
		functionSequence->addMultiChar("mov R13, RBP\nmov RBP, RSP\n");
	}
	Function(String* inpName, DynArray<ParamElement*>* inpParameterList)
	{
		name = new String(inpName);
		parameterList = new DynArray<ParamElement*>(*inpParameterList);
		functionSequence = new String();
		functionSequence->addMultiChar("mov R13, RBP\nmov RBP, RSP\n");
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

	void addParameter(VarElement* varToAdd)
	{
		if (localVarList == nullptr)
			localVarList = new DynArray<VarElement*>();
		localVarList->push_back(varToAdd);
	}

	void printParams()
	{
		int size = parameterList->size();
		for (int i = 0; i < size; i++)
		{
			std::cout << *(*parameterList)[i]->name << " " << *(*parameterList)[i]->dataType << " " << *(*parameterList)[i]->elemType << std::endl;
		}
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
				if (*(*parameterList)[i]->name == *param)
					return i + 1;
			}
		}
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

	int returnLocalVarSpace()
	{
		return localVarCount * 8; // пока тип только integer
	}
};