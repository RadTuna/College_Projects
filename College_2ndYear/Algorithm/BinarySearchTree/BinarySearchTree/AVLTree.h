#pragma once

#include "ITreeMap.h"

template<typename K, typename D>
struct AVLNode
{
	K Key;
	D Data;
	AVLNode<K, D>* Left = nullptr;
	AVLNode<K, D>* Right = nullptr;
	AVLNode<K, D>* Parent = nullptr;
};

template<typename K, typename D>
class AVLTree final : public ITreeMap<K, D>
{
public:

	explicit AVLTree() = default;
	~AVLTree() noexcept;

	bool Search(D* outData, const K& key) const override;
	bool Insert(const K& key, const D& data) override;
	bool Delete(const K& key) override;
	
	uint32_t GetSize() const override;
	uint32_t GetHeight() const override;
	void PrintTree() const override;

private:

	AVLNode<K, D>* FindMaxChildNode(AVLNode<K, D>* node) const;
	AVLNode<K, D>* FindMinChildNode(AVLNode<K, D>* node) const;
	
	void DeleteInternal(AVLNode<K ,D>* findNode);
	bool SearchNodeInternal(AVLNode<K, D>** outNode, const K& key) const;
	void PrintTreeInternal(AVLNode<K, D>* node) const;

	int32_t GetBalanceFactor(AVLNode<K, D>* node) const;
	uint32_t GetTreeHeight(AVLNode<K, D>* node) const;

	void LLRotation(AVLNode<K, D>* node);
	void RRRotation(AVLNode<K, D>* node);

	void CheckTreeImbalanced(AVLNode<K, D>* node);

	void SwapChildNode(AVLNode<K, D>* targetNode, AVLNode<K, D>* oldNode, AVLNode<K, D>* newNode);
	
private:

	AVLNode<K, D>* mRoot = nullptr;
	uint32_t mSize = 0;
	
};

// AVL Tree implementation
#include "AVLTree.hpp"
