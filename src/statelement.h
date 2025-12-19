#pragma once
#include "string.h"

class StatElement
{
public:
	String* type = nullptr;
	String* value = nullptr;

	StatElement(String* inpType, String* inpValue)
	{
		type = new String(inpType);
		value = new String(inpValue);
	}
	~StatElement()
	{
		delete type;
		delete value;
	}
};