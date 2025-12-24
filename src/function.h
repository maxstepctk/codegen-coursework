#pragma once
#include "paramelement.h"
#include "string.h"
#include "dynarray.h"

class Function
{
private:
	String* name = nullptr;
	DynArray<ParamElement*>* parameterList = nullptr;
public:
	Function(String* inpName)
	{
		name = new String(inpName);
	}
	Function(String* inpName, DynArray<ParamElement*>* inpParameterList)
	{
		name = new String(inpName);
		parameterList = new DynArray<ParamElement*>(*inpParameterList);
	}
	~Function()
	{
		delete name;
		if (parameterList != nullptr)
			delete parameterList;
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

	const ParamElement* returnParam(String* param) 
	{
		int size = parameterList->size();
		for (int i = 0; i < size; i++)
		{
			if (*(*parameterList)[i]->name == *param)
				return (*parameterList)[i];
		}
		return nullptr;
	}

	int returnPlace(String* param)
	{
		int size = parameterList->size();
		for (int i = 0; i < size; i++)
		{
			if (*(*parameterList)[i]->name == *param)
				return i;
		}
		return 0;
	}

};