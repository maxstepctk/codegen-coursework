#include "codegen-coursework.h"

//bool readBinTreeFromFile(BinaryTree<String>* tree, const char* filename)
//{
//	FILE* fp = fopen(filename, "r");
//	char* letter = new char('a');
//	String* tempstr = new String();
//	bool isReading = false;
//	bool treeSide = false;
//	if (fp)
//	{
//		while ((fgets(letter, 1, fp)) != NULL)
//		{
//			if (*letter == '(')
//			{
//				treeSide = false;
//				if (isReading)
//				{
//
//					delete tempstr;
//					tempstr = new String();
//				}
//				else
//				{
//					isReading = true;
//				}
//			}
//			else if (*letter == ')')
//			{
//				treeSide = true;
//				tree->addElement(*tempstr, treeSide);
//			}
//			else
//				tempstr->addSym(*letter);
//		}
//		return true;
//	}
//	else
//		return false;
//}

//void readBinTreeFromFile(BinaryTree<String>* tree, const char* filename)
//{
//	Stack<BinaryTree<String>*> s1;
//	s1.push(tree);
//	FILE* fp = fopen(filename, "r");
//	char* letter = new char('a');
//	String* tempstr = new String();
//	bool notStart = false;
//	bool lastIsClose = false;
//	if (fp)
//	{
//		while ((fgets(letter, 1, fp)) != NULL)
//		{
//			if (*letter == '(')
//			{
//				if (notStart)
//				{
//					s1.top()->writeToHead(*tempstr);
//					if (tempstr != nullptr)
//						delete tempstr;
//					tempstr = new String();
//					if (lastIsClose)
//						s1.push(s1.top()->right);
//					else
//						s1.push(s1.top()->left);
//				}
//			}
//			else if (*letter = ')')
//			{
//				s1.top()->writeToHead(*tempstr);
//				delete tempstr;
//				s1.pop();
//			}
//			else
//				tempstr->addSym(*letter);
//		}
//	}
//}

int main()
{
	const char* filename = "C:/Users/MaxStep/Downloads/Telegram Desktop/syntax_tree (3).txt";
	String str1;
	str1.addSym('a');
	str1.addSym('b');
	str1[0] = 'c';
	//BinaryTree<String>* sintaxTree = new BinaryTree<String>();
	//readBinTreeFromFile(sintaxTree, filename);
	SyntaxTree* tree1 = new SyntaxTree(filename);
	std::cout << *tree1 << std::endl;
	return 0;
}
