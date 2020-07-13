#pragma once

#include "BinarySearchTree.h"
#include <iostream>

template <typename K, typename D>
BinarySearchTree<K, D>::~BinarySearchTree() noexcept
{
	while (mRoot != nullptr)
	{
		DeleteInternal(mRoot);
	}
}

template <typename K, typename D>
bool BinarySearchTree<K, D>::Search(D* outData, const K& key) const
{
	BSTNode<K, D>* findNode = nullptr;
	if (SearchNodeInternal(&findNode, key))
	{
		*outData = findNode->Data;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool BinarySearchTree<K, D>::Insert(const K& key, const D& data)
{
	BSTNode<K, D>* newNode = new BSTNode<K, D>{ key, data, nullptr, };

	if (mRoot == nullptr)
	{
		mRoot = newNode;
		++mSize;
		return true;
	}

	BSTNode<K, D>* findNode = nullptr;
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

		++mSize;
		return true;
	}

	return false;
}

template <typename K, typename D>
bool BinarySearchTree<K, D>::Delete(const K& key)
{
	BSTNode<K, D>* findNode = nullptr;
	if (!SearchNodeInternal(&findNode, key))
	{
		return false;
	}

	DeleteInternal(findNode);
	return true;
}

template <typename K, typename D>
BSTNode<K, D>* BinarySearchTree<K, D>::FindMaxChildNode(BSTNode<K, D>* node) const
{
	BSTNode<K, D>* currentNode = node;
	while (currentNode->Right != nullptr)
	{
		currentNode = currentNode->Right;
	}

	return currentNode;
}

template <typename K, typename D>
BSTNode<K, D>* BinarySearchTree<K, D>::FindMinChildNode(BSTNode<K, D>* node) const
{
	BSTNode<K, D>* currentNode = node;
	while (currentNode->Left != nullptr)
	{
		currentNode = currentNode->Left;
	}

	return currentNode;
}

template <typename K, typename D>
uint32_t BinarySearchTree<K, D>::GetSize() const
{
	return mSize;
}

template <typename K, typename D>
void BinarySearchTree<K, D>::PrintTree() const
{
	PrintTreeInternal(mRoot);
}

template <typename K, typename D>
void BinarySearchTree<K, D>::DeleteInternal(BSTNode<K, D>* findNode)
{
	BSTNode<K, D>* parentNode = findNode->Parent;

	if (findNode->Left == nullptr || findNode->Right == nullptr)
	{
		BSTNode<K, D>* childNode = nullptr;

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
		BSTNode<K, D>* minNode = FindMinChildNode(findNode->Right);
		BSTNode<K, D>* parentMinNode = minNode->Parent;

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
bool BinarySearchTree<K, D>::SearchNodeInternal(BSTNode<K, D>** outNode, const K& key) const
{
	if (mRoot == nullptr)
	{
		*outNode = nullptr;
		return false;
	}

	BSTNode<K, D>* currentNode = mRoot;
	BSTNode<K, D>* prevNode = nullptr;
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
void BinarySearchTree<K, D>::PrintTreeInternal(BSTNode<K, D>* node) const
{
	if (node != nullptr)
	{
		PrintTreeInternal(node->Left);
		std::cout << "Key : " << node->Key << " / Data : " << node->Data << std::endl;
		PrintTreeInternal(node->Right);
	}
}


