#pragma once
#include "string.h"

class VarElement
{
public:
	String* name = nullptr;
	String* type = nullptr;

	VarElement() {}

	VarElement(String* inpName, String* inpType)
	{
		name = new String(inpName);
		type = new String(inpType);
	}
	~VarElement()
	{
		delete name;
		delete type;
	}
};