#include "main.h"

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "C:\\Users\\MaxStep\\source\\repos\\syntax3\\syntax\\syntax_tree.txt";
	Codegen* codegen = new Codegen();
	codegen->genCode(filename);
	delete codegen;
	return 0;
}