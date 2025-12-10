#include "codegen-coursework.h"
#include <locale>

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "C:\\Users\\MaxStep\\source\\repos\\syntax\\syntax\\syntax_tree.txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	std::cout << *tree1 << std::endl;
	return 0;
}
