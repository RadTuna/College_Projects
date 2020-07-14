#pragma once

#include "ITreeMap.h"

enum class ERBColor
{
	Red,
	Black
};

template<typename K, typename D>
struct RBNode
{
	K Key;
	D Data;
	ERBColor Color;
	RBNode<K, D>* Left = nullptr;
	RBNode<K, D>* Right = nullptr;
	RBNode<K, D>* Parent = nullptr;
};

template<typename K, typename D>
class RedBlackTree final : public ITreeMap<K, D>
{
public:

	explicit RedBlackTree() = default;
	~RedBlackTree() noexcept;

	bool Search(D* outData, const K& key) const override;
	bool Insert(const K& key, const D& data) override;
	bool Delete(const K& key) override;
	
	uint32_t GetSize() const override;
	uint32_t GetHeight() const override;
	void PrintTree() const override;

private:

	RBNode<K, D>* FindMaxChildNode(RBNode<K, D>* node) const;
	RBNode<K, D>* FindMinChildNode(RBNode<K, D>* node) const;
	
	void DeleteInternal(RBNode<K ,D>* findNode);
	bool SearchNodeInternal(RBNode<K, D>** outNode, const K& key) const;
	void PrintTreeInternal(RBNode<K, D>* node) const;

	int32_t GetBalanceFactor(RBNode<K, D>* node) const;
	uint32_t GetTreeHeight(RBNode<K, D>* node) const;

	void LLRotation(RBNode<K, D>* node);
	void RRRotation(RBNode<K, D>* node);

	void CheckParentColor(RBNode<K, D>* node);
	void Restructuring(RBNode<K, D>* node);
	void Recoloring(RBNode<K, D>* node);
	RBNode<K, D>* GetSiblingNode(RBNode<K, D>* node);
	void SwapChildNode(RBNode<K, D>* targetNode, RBNode<K, D>* oldNode, RBNode<K, D>* newNode);
	
private:

	RBNode<K, D>* mRoot = nullptr;
	uint32_t mSize = 0;
	
};

// AVL Tree implementation
#include "RedBlackTree.hpp"
