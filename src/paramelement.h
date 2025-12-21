#pragma once
#include "string.h"

class ParamElement
{
public:
	String* name = nullptr;
	String* dataType = nullptr;
	int* elemType = nullptr; // 1 - по значению, 2 - var, 3 - const

	ParamElement() {}

	ParamElement(String* inpName, String* inpDataType, int inpElemType)
	{
		name = new String(inpName);
		dataType = new String(inpDataType);
		elemType = new int(inpElemType);
	}
	~ParamElement()
	{
		delete name;
		delete dataType;
		delete elemType;
	}
};