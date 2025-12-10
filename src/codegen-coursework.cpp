#include "codegen-coursework.h"
#include <locale>

int main()
{
	setlocale(LC_ALL, "Rus");
	const char* filename = "C:\\Users\\MaxStep\\source\\repos\\syntax\\syntax\\syntax_tree.txt";
	String str1;
	str1.addSym('a');
	str1.addSym('b');
	str1[0] = 'c';
	//BinaryTree<String>* sintaxTree = new BinaryTree<String>();
	//readBinTreeFromFile(sintaxTree, filename);
	SyntaxTree* tree1 = new SyntaxTree(filename);
	//SyntaxTree* tree2 = new SyntaxTree();
	//Stack<SyntaxTree*> s1;
	//std::cout << s1.top() << ", size = " << s1.size() << std::endl;
	//s1.push(tree1);
	//std::cout << s1.top() << ", size = " << s1.size() << std::endl;
	//s1.push(tree2);
	//std::cout << s1.top() << ", size = " << s1.size() << std::endl;
	//s1.pop();
	//std::cout << s1.top() << ", size = " << s1.size() << std::endl;
	//SyntaxTree* editingTree = s1.top();
	//editingTree->info = &str1;
	//std::cout << *tree1->info << std::endl;
	//s1.pop();
	//std::cout << s1.top() << ", size = " << s1.size() << std::endl;
	String str2;
	str2.addSym('L');
	String str3;
	str3.addString(&str1);
	str3.addString(&str2);
	std::cout << str3 << std::endl;
	std::cout << *tree1 << std::endl;
	return 0;
}
