#pragma once
#include "string.h"
#include "varelement.h"

class ConstElement : public VarElement
{
public:
	String* value = nullptr;

	ConstElement(String* inpName, String* inpType, String* inpValue) : VarElement(inpName, inpType)
	{
		value = new String(inpValue);
	}

	~ConstElement()
	{
		delete value;
	}
};