#include "codegen-coursework.h"
#include <locale>

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "D:\\MaxStep\\Downloads\\Telegram Desktop\\syntax_tree (3).txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	std::cout << *tree1 << std::endl;
	return 0;
}
