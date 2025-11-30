#include "codegen-coursework.h"

bool readBinTreeFromFile(BinaryTree<String>* tree, const char* filename)
{
	FILE* fp = fopen(filename, "r");
	char* letter = new char('a');
	String* tempstr = new String();
	bool isReading = false;
	bool treeSide = false;
	if (fp)
	{
		while ((fgets(letter, 1, fp)) != NULL)
		{
			if (*letter == '(')
			{
				treeSide = false;
				if (isReading)
				{

					delete tempstr;
					tempstr = new String();
				}
				else
				{
					isReading = true;
				}
			}
			else if (*letter == ')')
			{
				treeSide = true;
				tree->addElement(*tempstr, treeSide);
			}
			else
				tempstr->addBack(*letter);
		}
		return true;
	}
	else
		return false;
}

int main()
{
	const char* filename = "C:/Users/MaxStep/Downloads/Telegram Desktop/syntax_tree (2).txt";
	String str1;
	str1.addBack('a');
	str1.addBack('b');
	str1[0] = 'c';
	BinaryTree<String>* sintaxTree = new BinaryTree<String>();
	readBinTreeFromFile(sintaxTree, filename);
	std::cout << sintaxTree << std::endl;
	return 0;
}
