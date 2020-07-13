
#include <string>
#include <map>
#include <iostream>

#include "BinarySearchTree.h"

#define LINE_SEPARATOR std::cout << "\n===========================\n" << std::endl

enum class ETreeType
{
	BinarySearchTree
};

int main()
{
	std::map<ETreeType, TreeMap<int32_t, std::string>*> TreeMaps;

	TreeMaps[ETreeType::BinarySearchTree] = new BinarySearchTree<int32_t, std::string>();
	
	TreeMaps[ETreeType::BinarySearchTree]->Insert(57, "�׽�Ʈ ���� 57");
	TreeMaps[ETreeType::BinarySearchTree]->Insert(41, "�׽�Ʈ ���� 41");
	TreeMaps[ETreeType::BinarySearchTree]->Insert(11, "�׽�Ʈ ���� 11");
	TreeMaps[ETreeType::BinarySearchTree]->Insert(89, "�׽�Ʈ ���� 89");
	TreeMaps[ETreeType::BinarySearchTree]->Insert(54, "�׽�Ʈ ���� 54");

	TreeMaps[ETreeType::BinarySearchTree]->PrintTree();

	LINE_SEPARATOR;
	
	TreeMaps[ETreeType::BinarySearchTree]->Delete(11);
	TreeMaps[ETreeType::BinarySearchTree]->Delete(57);
	
	TreeMaps[ETreeType::BinarySearchTree]->PrintTree();

	std::string data;
	TreeMaps[ETreeType::BinarySearchTree]->Search(&data, 89);
	std::cout << data << std::endl;

	
	for (auto pair : TreeMaps)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	
	return 0;
}
