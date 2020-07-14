
#include <string>
#include <map>
#include <iostream>
#include <array>

#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "TimeCounter.h"

#define LINE_SEPARATOR std::cout << "\n===========================\n" << std::endl

#define INSERT_TEST(Type, TestCase) \
	TimeCounter::GetInstance()->CaptureStart(); \
	for (auto pair : TestCase) \
	{ \
		TreeMaps[Type]->Insert(pair.first, pair.second); \
	} \
	TimeCounter::GetInstance()->CaptureEnd() \

#define DELETE_TEST(Type, TestCase) \
	TimeCounter::GetInstance()->CaptureStart(); \
	for (int32_t key : TestCase) \
	{ \
		TreeMaps[Type]->Delete(key); \
	} \
	TimeCounter::GetInstance()->CaptureEnd() \

#define SEARCH_TEST(Type, TestCase, OutData) \
	TimeCounter::GetInstance()->CaptureStart(); \
	for (int32_t key : TestCase) \
	{ \
		TreeMaps[Type]->Search(&OutData, key); \
	} \
	TimeCounter::GetInstance()->CaptureEnd() \

enum class ETreeType
{
	BinarySearchTree,
	AVLTree,
	RedBlackTree
};

int main()
{
	std::array<std::pair<int32_t, std::string>, 10> insertTestCase = {
		std::pair<int32_t, std::string>(57, "테스트 문장 57"),
		std::pair<int32_t, std::string>(41, "테스트 문장 41"),
		std::pair<int32_t, std::string>(11, "테스트 문장 11"),
		std::pair<int32_t, std::string>(89, "테스트 문장 89"),
		std::pair<int32_t, std::string>(54, "테스트 문장 54"),
		std::pair<int32_t, std::string>(8, "테스트 문장 8"),
		std::pair<int32_t, std::string>(37, "테스트 문장 37"),
		std::pair<int32_t, std::string>(20, "테스트 문장 20"),
		std::pair<int32_t, std::string>(50, "테스트 문장 50"),
		std::pair<int32_t, std::string>(40, "테스트 문장 40")
	};
	
	std::array<int32_t, 10> searchTestCase = { 57, 41, 11, 89, 54,
		8, 37, 20, 50, 40 };
	std::array<int32_t, 5> deleteTestCase = { 11, 57, 97, 50, 89 };
	
	std::map<ETreeType, TreeMap<int32_t, std::string>*> TreeMaps;
	TreeMaps[ETreeType::BinarySearchTree] = new BinarySearchTree<int32_t, std::string>();
	TreeMaps[ETreeType::AVLTree] = new AVLTree<int32_t, std::string>();
	TimeCounter::CreateInstance();
	std::string dummy;

	// TEST_START

	std::cout << "BinarySearchTree" << std::endl;
	std::cout << "Insert" << std::endl;
	INSERT_TEST(ETreeType::BinarySearchTree, insertTestCase);
	TreeMaps[ETreeType::BinarySearchTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Search" << std::endl;
	SEARCH_TEST(ETreeType::BinarySearchTree, searchTestCase, dummy);
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Delete" << std::endl;
	DELETE_TEST(ETreeType::BinarySearchTree, deleteTestCase);
	TreeMaps[ETreeType::BinarySearchTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "AVL Tree" << std::endl;
	std::cout << "Insert" << std::endl;
	INSERT_TEST(ETreeType::AVLTree, insertTestCase);
	TreeMaps[ETreeType::AVLTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;
	
	std::cout << "Search" << std::endl;
	SEARCH_TEST(ETreeType::BinarySearchTree, searchTestCase, dummy);
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Delete" << std::endl;
	DELETE_TEST(ETreeType::AVLTree, deleteTestCase);
	TreeMaps[ETreeType::AVLTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	// TEST_END
	

	TimeCounter::DeleteInstance();
	for (auto pair : TreeMaps)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	
	return 0;
}
