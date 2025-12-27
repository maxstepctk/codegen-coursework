#pragma once
#include "string.h"

class StatElement
{
public:
	String* type = nullptr;
	String* value = nullptr;
	bool isPointer = false;

	StatElement() {}

	StatElement(String* inpType, String* inpValue, bool isPointer)
	{
		type = new String(inpType);
		value = new String(inpValue);
		isPointer = isPointer;
	}
	~StatElement()
	{
		delete type;
		delete value;
	}
};