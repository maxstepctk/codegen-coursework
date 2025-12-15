#include "codegen-coursework.h"
#include <locale>

String progname;
RegState regs;

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "C:\\Users\\MaxStep\\source\\repos\\syntax\\syntax\\syntax_tree.txt";
	SyntaxTree* tree1 = new SyntaxTree(filename);
	if (tree1->size() != 0)
	{
		std::cout << "Дерево успешно прочитано.\nРазмер дерева: " << tree1->size() << std::endl;
		std::cout << "Прочитанное дерево:\n" << *tree1 << std::endl;
	}
	else
		std::cout << "Ошибка чтения дерева." << std::endl;
	std::cout << regs.returnRegName(10) << std::endl;
	return 0;
}
