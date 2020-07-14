
#include <string>
#include <map>
#include <iostream>
#include <array>
#include <random>

#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "RedBlackTree.h"
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
	enum { TEST_CASE_SIZE = 10000, SEARCH_CASE_DIV_FACTOR = 100 };
	
	std::random_device rngDev;
	std::uniform_int_distribution<int32_t> uniformDist(0, TEST_CASE_SIZE);
	std::vector<std::pair<int32_t, std::string>> insertTestCase;
	insertTestCase.reserve(TEST_CASE_SIZE + 16);
	for (uint32_t i = 0; i < TEST_CASE_SIZE; ++i)
	{
		int32_t rngValue = uniformDist(rngDev);
		insertTestCase.emplace_back(rngValue, "테스트 문장");
	}

	std::vector<int32_t> searchTestCase;
	searchTestCase.reserve(TEST_CASE_SIZE / SEARCH_CASE_DIV_FACTOR + 16);
	const uint32_t searchTestCaseSize = TEST_CASE_SIZE / SEARCH_CASE_DIV_FACTOR;
	for (uint32_t i = 0; i < searchTestCaseSize; ++i)
	{
		searchTestCase.emplace_back(insertTestCase[i].first);
	}
	
	std::vector<int32_t> deleteTestCase;
	deleteTestCase.reserve(TEST_CASE_SIZE + 16);
	for (std::pair<int32_t, std::string> pair : insertTestCase)
	{
		deleteTestCase.emplace_back(pair.first);
	}
	
	std::shuffle(insertTestCase.begin(), insertTestCase.end(), rngDev);
	std::shuffle(searchTestCase.begin(), searchTestCase.end(), rngDev);
	std::shuffle(deleteTestCase.begin(), deleteTestCase.end(), rngDev);

	std::sort(insertTestCase.begin(), insertTestCase.end());

	
	std::map<ETreeType, ITreeMap<int32_t, std::string>*> TreeMaps;
	TreeMaps[ETreeType::BinarySearchTree] = new BinarySearchTree<int32_t, std::string>();
	TreeMaps[ETreeType::AVLTree] = new AVLTree<int32_t, std::string>();
	TreeMaps[ETreeType::RedBlackTree] = new RedBlackTree<int32_t, std::string>();
	TimeCounter::CreateInstance();
	std::string dummy;

	// TEST_START

	std::cout << "BinarySearchTree" << std::endl;
	std::cout << "Insert" << std::endl;
	INSERT_TEST(ETreeType::BinarySearchTree, insertTestCase);
	//TreeMaps[ETreeType::BinarySearchTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	std::cout << "TreeHeight : " << TreeMaps[ETreeType::BinarySearchTree]->GetHeight() << std::endl;
	LINE_SEPARATOR;

	std::cout << "Search" << std::endl;
	SEARCH_TEST(ETreeType::BinarySearchTree, searchTestCase, dummy);
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Delete" << std::endl;
	DELETE_TEST(ETreeType::BinarySearchTree, deleteTestCase);
	//TreeMaps[ETreeType::BinarySearchTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "AVL Tree" << std::endl;
	std::cout << "Insert" << std::endl;
	INSERT_TEST(ETreeType::AVLTree, insertTestCase);
	//TreeMaps[ETreeType::AVLTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	std::cout << "TreeHeight : " << TreeMaps[ETreeType::AVLTree]->GetHeight() << std::endl;
	LINE_SEPARATOR;
	
	std::cout << "Search" << std::endl;
	SEARCH_TEST(ETreeType::AVLTree, searchTestCase, dummy);
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Delete" << std::endl;
	DELETE_TEST(ETreeType::AVLTree, deleteTestCase);
	//TreeMaps[ETreeType::AVLTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "RedBlack Tree" << std::endl;
	std::cout << "Insert" << std::endl;
	INSERT_TEST(ETreeType::RedBlackTree, insertTestCase);
	//TreeMaps[ETreeType::RedBlackTree]->PrintTree();
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	std::cout << "TreeHeight : " << TreeMaps[ETreeType::RedBlackTree]->GetHeight() << std::endl;
	LINE_SEPARATOR;
	
	std::cout << "Search" << std::endl;
	SEARCH_TEST(ETreeType::RedBlackTree, searchTestCase, dummy);
	std::cout << "Time : " << TimeCounter::GetInstance()->GetDeltaTime() << "ms" << std::endl;
	LINE_SEPARATOR;

	std::cout << "Delete" << std::endl;
	DELETE_TEST(ETreeType::RedBlackTree, deleteTestCase);
	//TreeMaps[ETreeType::RedBlackTree]->PrintTree();
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
