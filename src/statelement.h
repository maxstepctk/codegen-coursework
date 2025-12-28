#pragma once
#include "string.h"

class StatElement
{
public:
	String* type = nullptr;
	String* value = nullptr;
	bool isPointer = false;

	StatElement() {}

	StatElement(String* inpType, String* inpValue, bool inpIsPointer)
	{
		type = new String(inpType);
		value = new String(inpValue);
		isPointer = inpIsPointer;
	}
	~StatElement()
	{
		delete type;
		delete value;
	}
};