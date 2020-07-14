#pragma once

#include "AVLTree.h"
#include <iostream>
#include <algorithm>
#include <cassert>

template <typename K, typename D>
AVLTree<K, D>::~AVLTree() noexcept
{
	while (mRoot != nullptr)
	{
		DeleteInternal(mRoot);
	}
}

template <typename K, typename D>
bool AVLTree<K, D>::Search(D* outData, const K& key) const
{
	AVLNode<K, D>* findNode = nullptr;
	if (SearchNodeInternal(&findNode, key))
	{
		*outData = findNode->Data;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool AVLTree<K, D>::Insert(const K& key, const D& data)
{
	AVLNode<K, D>* newNode = new AVLNode<K, D>{ key, data, nullptr, };

	if (mRoot == nullptr)
	{
		mRoot = newNode;
		++mSize;
		return true;
	}

	AVLNode<K, D>* findNode = nullptr;
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
		CheckTreeImbalanced(findNode);
		
		++mSize;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool AVLTree<K, D>::Delete(const K& key)
{
	AVLNode<K, D>* findNode = nullptr;
	if (!SearchNodeInternal(&findNode, key))
	{
		return false;
	}

	AVLNode<K, D>* parentNode = findNode->Parent;
	DeleteInternal(findNode);

	if (parentNode != nullptr)
	{
		CheckTreeImbalanced(parentNode);
	}
	
	return true;
}

template <typename K, typename D>
AVLNode<K, D>* AVLTree<K, D>::FindMaxChildNode(AVLNode<K, D>* node) const
{
	AVLNode<K, D>* currentNode = node;
	while (currentNode->Right != nullptr)
	{
		currentNode = currentNode->Right;
	}

	return currentNode;
}

template <typename K, typename D>
AVLNode<K, D>* AVLTree<K, D>::FindMinChildNode(AVLNode<K, D>* node) const
{
	AVLNode<K, D>* currentNode = node;
	while (currentNode->Left != nullptr)
	{
		currentNode = currentNode->Left;
	}

	return currentNode;
}

template <typename K, typename D>
uint32_t AVLTree<K, D>::GetSize() const
{
	return mSize;
}

template <typename K, typename D>
uint32_t AVLTree<K, D>::GetHeight() const
{
	return GetTreeHeight(mRoot);
}

template <typename K, typename D>
void AVLTree<K, D>::PrintTree() const
{
	PrintTreeInternal(mRoot);
}

template <typename K, typename D>
void AVLTree<K, D>::DeleteInternal(AVLNode<K, D>* findNode)
{
	AVLNode<K, D>* parentNode = findNode->Parent;

	if (findNode->Left == nullptr || findNode->Right == nullptr)
	{
		AVLNode<K, D>* childNode = nullptr;

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
		AVLNode<K, D>* minNode = FindMinChildNode(findNode->Right);
		AVLNode<K, D>* parentMinNode = minNode->Parent;

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
bool AVLTree<K, D>::SearchNodeInternal(AVLNode<K, D>** outNode, const K& key) const
{
	if (mRoot == nullptr)
	{
		*outNode = nullptr;
		return false;
	}

	AVLNode<K, D>* currentNode = mRoot;
	AVLNode<K, D>* prevNode = nullptr;
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
void AVLTree<K, D>::PrintTreeInternal(AVLNode<K, D>* node) const
{
	if (node != nullptr)
	{
		PrintTreeInternal(node->Left);
		std::cout << "Key : " << node->Key << " / Data : " << node->Data << std::endl;
		PrintTreeInternal(node->Right);
	}
}

template <typename K, typename D>
int32_t AVLTree<K, D>::GetBalanceFactor(AVLNode<K, D>* node) const
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
uint32_t AVLTree<K, D>::GetTreeHeight(AVLNode<K, D>* node) const
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
void AVLTree<K, D>::LLRotation(AVLNode<K, D>* node)
{
	assert(node->Right != nullptr);
	
	AVLNode<K, D>* firstNode = node;
	AVLNode<K, D>* secondNode = node->Right;

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
}

template <typename K, typename D>
void AVLTree<K, D>::RRRotation(AVLNode<K, D>* node)
{
	assert(node->Left != nullptr);
	
	AVLNode<K, D>* firstNode = node;
	AVLNode<K, D>* secondNode = node->Left;

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
}

template <typename K, typename D>
void AVLTree<K, D>::CheckTreeImbalanced(AVLNode<K, D>* node)
{
	const int32_t balanceFactor = GetBalanceFactor(node);
	if (std::abs(balanceFactor) >= 2) // is imbalanced
	{
		uint32_t leftHeight = 0;
		uint32_t rightHeight = 0;
		if (balanceFactor > 0) // left
		{
			leftHeight = GetTreeHeight(node->Left->Left);
			rightHeight = GetTreeHeight(node->Left->Right);
			if (leftHeight > rightHeight) // left = LL imbalanced
			{
				RRRotation(node);
			}
			else // right = LR imbalanced
			{
				LLRotation(node->Left);
				RRRotation(node);
			}
		}
		else // right
		{
			leftHeight = GetTreeHeight(node->Right->Left);
			rightHeight = GetTreeHeight(node->Right->Right);
			if (leftHeight > rightHeight) // left = RL imbalanced
			{
				RRRotation(node->Right);
				LLRotation(node);
			}
			else // right = RR imbalanced
			{
				LLRotation(node);
			}
		}
	}

	if (node->Parent != nullptr)
	{
		CheckTreeImbalanced(node->Parent);
	}
}

template <typename K, typename D>
void AVLTree<K, D>::SwapChildNode(AVLNode<K, D>* targetNode, AVLNode<K, D>* oldNode, AVLNode<K, D>* newNode)
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


