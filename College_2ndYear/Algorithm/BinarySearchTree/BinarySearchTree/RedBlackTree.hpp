#pragma once

#include "RedBlackTree.h"
#include <iostream>
#include <algorithm>
#include <cassert>

template <typename K, typename D>
RedBlackTree<K, D>::~RedBlackTree() noexcept
{
	while (mRoot != nullptr)
	{
		DeleteInternal(mRoot);
	}
}

template <typename K, typename D>
bool RedBlackTree<K, D>::Search(D* outData, const K& key) const
{
	RBNode<K, D>* findNode = nullptr;
	if (SearchNodeInternal(&findNode, key))
	{
		*outData = findNode->Data;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool RedBlackTree<K, D>::Insert(const K& key, const D& data)
{
	RBNode<K, D>* newNode = new RBNode<K, D>{ key, data, ERBColor::Red, nullptr, };

	if (mRoot == nullptr)
	{
		newNode->Color = ERBColor::Black;
		mRoot = newNode;
		++mSize;
		return true;
	}

	RBNode<K, D>* findNode = nullptr;
	if (!SearchNodeInternal(&findNode, key))
	{
		if (findNode->Key > newNode->Key)
		{
			findNode->Left = newNode;
		}
		else
		{
			findNode->Right = newNode;
		}
		newNode->Parent = findNode;
		CheckParentColor(newNode);

		++mSize;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool RedBlackTree<K, D>::Delete(const K& key)
{
	RBNode<K, D>* findNode = nullptr;
	if (!SearchNodeInternal(&findNode, key))
	{
		return false;
	}

	bool isLeftNode = false;
	RBNode<K, D>* parentNode = findNode->Parent;
	RBNode<K, D>* swapNode = nullptr;
	if (parentNode != nullptr)
	{
		if (parentNode->Left == findNode)
		{
			swapNode = parentNode->Left;
		}
		else
		{
			swapNode = parentNode->Right;
		}
	}

	DeleteInternal(findNode);

	if (swapNode != nullptr && swapNode->Color == ERBColor::Red)
	{
		//CheckParentColor(swapNode);
	}

	return true;
}

template <typename K, typename D>
RBNode<K, D>* RedBlackTree<K, D>::FindMaxChildNode(RBNode<K, D>* node) const
{
	RBNode<K, D>* currentNode = node;
	while (currentNode->Right != nullptr)
	{
		currentNode = currentNode->Right;
	}

	return currentNode;
}

template <typename K, typename D>
RBNode<K, D>* RedBlackTree<K, D>::FindMinChildNode(RBNode<K, D>* node) const
{
	RBNode<K, D>* currentNode = node;
	while (currentNode->Left != nullptr)
	{
		currentNode = currentNode->Left;
	}

	return currentNode;
}

template <typename K, typename D>
uint32_t RedBlackTree<K, D>::GetSize() const
{
	return mSize;
}

template <typename K, typename D>
uint32_t RedBlackTree<K, D>::GetHeight() const
{
	return GetTreeHeight(mRoot);
}

template <typename K, typename D>
void RedBlackTree<K, D>::PrintTree() const
{
	PrintTreeInternal(mRoot);
}

template <typename K, typename D>
void RedBlackTree<K, D>::DeleteInternal(RBNode<K, D>* findNode)
{
	RBNode<K, D>* parentNode = findNode->Parent;

	if (findNode->Left == nullptr || findNode->Right == nullptr)
	{
		RBNode<K, D>* childNode = nullptr;

		if (findNode->Left == nullptr)
		{
			childNode = findNode->Right;
		}
		else
		{
			childNode = findNode->Left;
		}

		if (parentNode != nullptr)
		{
			if (childNode != nullptr)
			{
				childNode->Parent = parentNode;
			}

			if (findNode == parentNode->Left)
			{
				parentNode->Left = childNode;
			}
			else
			{
				parentNode->Right = childNode;
			}
		}
		else
		{
			if (childNode != nullptr)
			{
				childNode->Parent = nullptr;
			}

			mRoot = childNode;
		}

		delete findNode;
		findNode = nullptr;
	}
	else
	{
		RBNode<K, D>* minNode = FindMinChildNode(findNode->Right);
		RBNode<K, D>* parentMinNode = minNode->Parent;

		if (minNode != findNode->Right)
		{
			parentMinNode->Left = minNode->Right;
			if (minNode->Right != nullptr)
			{
				minNode->Right->Parent = parentMinNode;
			}
		}
		else
		{
			findNode->Right = minNode->Right;
			if (minNode->Right != nullptr)
			{
				minNode->Right->Parent = findNode;
			}
		}

		findNode->Key = minNode->Key;
		findNode->Data = minNode->Data;

		delete minNode;
		minNode = nullptr;
	}

	--mSize;
}

template <typename K, typename D>
bool RedBlackTree<K, D>::SearchNodeInternal(RBNode<K, D>** outNode, const K& key) const
{
	if (mRoot == nullptr)
	{
		*outNode = nullptr;
		return false;
	}

	RBNode<K, D>* currentNode = mRoot;
	RBNode<K, D>* prevNode = nullptr;
	while (currentNode != nullptr)
	{
		if (currentNode->Key == key)
		{
			*outNode = currentNode;
			return true;
		}

		prevNode = currentNode;
		if (currentNode->Key > key)
		{
			currentNode = currentNode->Left;
		}
		else
		{
			currentNode = currentNode->Right;
		}
	}

	*outNode = prevNode;
	return false;
}

template <typename K, typename D>
void RedBlackTree<K, D>::PrintTreeInternal(RBNode<K, D>* node) const
{
	if (node != nullptr)
	{
		PrintTreeInternal(node->Left);
		std::cout << "Key : " << node->Key << " / Data : " << node->Data << std::endl;
		PrintTreeInternal(node->Right);
	}
}

template <typename K, typename D>
int32_t RedBlackTree<K, D>::GetBalanceFactor(RBNode<K, D>* node) const
{
	uint32_t leftHeight = 0;
	if (node->Left != nullptr)
	{
		leftHeight = GetTreeHeight(node->Left);
	}

	uint16_t rightHeight = 0;
	if (node->Right != nullptr)
	{
		rightHeight = GetTreeHeight(node->Right);
	}

	return leftHeight - rightHeight;
}

template <typename K, typename D>
uint32_t RedBlackTree<K, D>::GetTreeHeight(RBNode<K, D>* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	int32_t leftHeight = 0;
	if (node->Left != nullptr)
	{
		leftHeight = GetTreeHeight(node->Left);
	}

	int32_t rightHeight = 0;
	if (node->Right != nullptr)
	{
		rightHeight = GetTreeHeight(node->Right);
	}

	return std::max(leftHeight, rightHeight) + 1;
}

template <typename K, typename D>
void RedBlackTree<K, D>::LLRotation(RBNode<K, D>* node)
{
	assert(node->Right != nullptr);

	RBNode<K, D>* firstNode = node;
	RBNode<K, D>* secondNode = node->Right;

	firstNode->Right = secondNode->Left;
	if (secondNode->Left != nullptr)
	{
		secondNode->Left->Parent = firstNode;
	}

	secondNode->Left = firstNode;
	secondNode->Parent = firstNode->Parent;
	firstNode->Parent = secondNode;

	if (node == mRoot)
	{
		mRoot = secondNode;
	}
	else
	{
		SwapChildNode(secondNode->Parent, firstNode, secondNode);
	}

	firstNode->Color = ERBColor::Red;
	secondNode->Color = ERBColor::Black;
}

template <typename K, typename D>
void RedBlackTree<K, D>::RRRotation(RBNode<K, D>* node)
{
	assert(node->Left != nullptr);

	RBNode<K, D>* firstNode = node;
	RBNode<K, D>* secondNode = node->Left;

	firstNode->Left = secondNode->Right;
	if (secondNode->Right != nullptr)
	{
		secondNode->Right->Parent = firstNode;
	}

	secondNode->Right = firstNode;
	secondNode->Parent = firstNode->Parent;
	firstNode->Parent = secondNode;

	if (node == mRoot)
	{
		mRoot = secondNode;
	}
	else
	{
		SwapChildNode(secondNode->Parent, firstNode, secondNode);
	}

	firstNode->Color = ERBColor::Red;
	secondNode->Color = ERBColor::Black;
}

template <typename K, typename D>
void RedBlackTree<K, D>::CheckParentColor(RBNode<K, D>* node)
{
	assert(node->Color == ERBColor::Red); // must be red node (new node)
	assert(node->Parent != nullptr);

	if (node->Parent->Color == ERBColor::Red)
	{
		RBNode<K, D>* uncleNode = GetSiblingNode(node->Parent);
		if (uncleNode == nullptr || uncleNode->Color == ERBColor::Black) // re-structuring // null node == black node
		{
			Restructuring(node);
		}
		else // re-coloring
		{
			Recoloring(node);
		}
	}
}

template <typename K, typename D>
void RedBlackTree<K, D>::Restructuring(RBNode<K, D>* node)
{
	RBNode<K, D>* parentNode = node->Parent;
	RBNode<K, D>* grandParentNode = parentNode->Parent;
	assert(parentNode != nullptr);
	assert(grandParentNode != nullptr);

	if (grandParentNode->Left == parentNode) // left
	{
		if (parentNode->Left == node) // LL imbalanced
		{
			RRRotation(grandParentNode);
		}
		else // LR imbalanced
		{
			LLRotation(parentNode);
			RRRotation(grandParentNode);
		}
	}
	else // right
	{
		if (parentNode->Right == node) // RR imbalanced
		{
			LLRotation(grandParentNode);
		}
		else // RL imbalanced
		{
			RRRotation(parentNode);
			LLRotation(grandParentNode);
		}
	}
}

template <typename K, typename D>
void RedBlackTree<K, D>::Recoloring(RBNode<K, D>* node)
{
	RBNode<K, D>* parentNode = node->Parent;
	RBNode<K, D>* grandParentNode = parentNode->Parent;
	assert(parentNode != nullptr);
	assert(grandParentNode != nullptr);

	RBNode<K, D>* uncleNode = GetSiblingNode(parentNode);

	parentNode->Color = ERBColor::Black;
	uncleNode->Color = ERBColor::Black;

	if (mRoot == grandParentNode)
	{
		grandParentNode->Color = ERBColor::Black;
	}
	else
	{
		grandParentNode->Color = ERBColor::Red;
		if (grandParentNode->Parent != nullptr &&
			grandParentNode->Parent->Color == ERBColor::Red)
		{
			CheckParentColor(grandParentNode);
		}
	}
}

template <typename K, typename D>
RBNode<K, D>* RedBlackTree<K, D>::GetSiblingNode(RBNode<K, D>* node)
{
	if (node->Parent == nullptr)
	{
		return nullptr;
	}

	if (node->Parent->Left == node)
	{
		return node->Parent->Right;
	}
	else
	{
		return node->Parent->Left;
	}
}

template <typename K, typename D>
void RedBlackTree<K, D>::SwapChildNode(RBNode<K, D>* targetNode, RBNode<K, D>* oldNode, RBNode<K, D>* newNode)
{
	if (targetNode->Left == oldNode)
	{
		targetNode->Left = newNode;
	}
	else if (targetNode->Right == oldNode)
	{
		targetNode->Right = newNode;
	}
	else
	{
		assert(false);
	}
}

