#pragma once

#include "ITreeMap.h"

template<typename K, typename D>
struct BSTNode
{
	K Key;
	D Data;
	BSTNode<K, D>* Left = nullptr;
	BSTNode<K, D>* Right = nullptr;
	BSTNode<K, D>* Parent = nullptr;
};

template<typename K, typename D>
class BinarySearchTree final : public ITreeMap<K, D>
{
public:

	explicit BinarySearchTree() = default;
	~BinarySearchTree() noexcept;

	bool Search(D* outData, const K& key) const override;
	bool Insert(const K& key, const D& data) override;
	bool Delete(const K& key) override;
	
	uint32_t GetSize() const override;
	uint32_t GetHeight() const override;
	void PrintTree() const override;

private:

	BSTNode<K, D>* FindMaxChildNode(BSTNode<K, D>* node) const;
	BSTNode<K, D>* FindMinChildNode(BSTNode<K, D>* node) const;
	
	void DeleteInternal(BSTNode<K ,D>* findNode);
	bool SearchNodeInternal(BSTNode<K, D>** outNode, const K& key) const;
	void PrintTreeInternal(BSTNode<K, D>* node) const;

	int32_t GetTreeHeight(BSTNode<K, D>* node) const;
	
private:

	BSTNode<K, D>* mRoot = nullptr;
	uint32_t mSize = 0;
	
};

// Template implementation
#include "BinarySerachTree.hpp"
