#pragma once
#include "string.h"
/*
Процессор на базе архитектуры x86_64 имеет 16 регистров:
Расширенные до 64 бит регистры классической x86:
RAX (Accumulator): для арифметических операций
RCX (Counter): для хранения счетчика цикла
RDX (Data): для арифметических операций и операций ввода-вывода
RBX (Base): указатель на данные
RSP (Stack pointer): указатель на верхушку стека
RBP (Base pointer): указатель на базу стека внутри функции
RSI (Source index): указатель на источник при операциях с массивом
RDI (Destination index): указатель на место назначения в операциях с массивами
Новые, появившиеся в x86_64 регистры: R8-R15
*/

class RegState
{
private:
	String regList[16];
	String regName[16];

public:
	RegState()
	{
		regName[0].addMultiChar("RAX");
		regName[1].addMultiChar("RCX");
		regName[2].addMultiChar("RDX");
		regName[3].addMultiChar("RBX");
		regName[4].addMultiChar("RSP");
		regName[5].addMultiChar("RBP");
		regName[6].addMultiChar("RSI");
		regName[7].addMultiChar("RDI");

		for (int i = 8; i <= 15; i++)
		{
			char buf[4];
			sprintf(buf, "R%d", i);
			regName[i].addMultiChar(buf);
		}
	}

	~RegState() {}

	int nameInWhichReg(String* varOrCharName)
	{
		for (int i = 0; i < 15; i++)
			if ((*varOrCharName) == regList[i])
				return i;
		return 16;
	}
	String& nameInWhichReg(int regnum)
	{
		if ((regnum >= 0) && (regnum <= 15))
			return regList[regnum];
		else
		{
			String outStr;
			outStr.addMultiChar("Incorrect register number");
			return outStr;
		}
	}
	String& returnRegName(int regnum)
	{
		if ((regnum >= 0) && (regnum <= 15))
			return regName[regnum];
		else
		{
			String* tempStr = new String();
			tempStr->addMultiChar("Incorrect register number");
			String outStr(tempStr);
			delete tempStr;
			return outStr;
		}
	}
};