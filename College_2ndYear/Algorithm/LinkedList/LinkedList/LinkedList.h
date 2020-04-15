#pragma once

#include <iostream>

template<typename Type>
class LinkedList
{
public:

	using uint32 = unsigned int;
	struct Node
	{
		Type Data;
		Node* Next = nullptr;
	};
	
public:

	LinkedList() = default;
	~LinkedList() = default;


	void Insert(uint32 pos, const Type& element);
	void Remove(uint32 pos);
	Type& Retrieve(uint32 pos) const;

	bool IsEmpty() { return mFront == nullptr; }

	void Print() const;

private:

	Node* GetPosPtr(uint32 pos, Node** prev = nullptr);

private:

	Node* mFront = nullptr;
	uint32 mSize = 0;
	
};

template <typename Type>
void LinkedList<Type>::Insert(uint32 pos, const Type& element)
{
	if (mSize < pos)
	{
		return;
	}
	
	Node* curNode = nullptr;
	if (pos == 0)
	{
		curNode = mFront;

		if (curNode == nullptr)
		{
			Node* newNode = new Node{ element, nullptr };
			mFront = newNode;
		}
		else
		{
			Node* newNode = new Node{ element, curNode };
			mFront = newNode;
		}

		++mSize;
		return;
	}
	else
	{
		curNode = GetPosPtr(pos - 1);
	}
	
	if (curNode == nullptr)
	{
		return;
	}

	Node* newNode = new Node{ element, curNode->Next };
	curNode->Next = newNode;

	++mSize;
}

template <typename Type>
void LinkedList<Type>::Remove(uint32 pos)
{
	if (mSize <= pos)
	{
		return;
	}
	if (IsEmpty())
	{
		return;
	}

	Node* curNode = nullptr;
	Node* prevNode = nullptr;
	if (pos == 0)
	{
		Node* pendingNode = mFront;
		mFront = mFront->Next;
		delete pendingNode;
		
		--mSize;
		return;
	}
	else
	{
		curNode = GetPosPtr(pos, &prevNode);
	}

	if (curNode == nullptr)
	{
		return;
	}

	prevNode->Next = curNode->Next;
	delete curNode;

	--mSize;
}

template <typename Type>
Type& LinkedList<Type>::Retrieve(uint32 pos) const
{
	Node* targetNode = GetPosPtr(pos);
	if (targetNode == nullptr)
	{
		return Type();
	}

	return targetNode->Data;
}

template <typename Type>
void LinkedList<Type>::Print() const
{
	std::cout << "{ ";

	Node* loopNode = mFront;
	for (;loopNode != nullptr;)
	{
		std::cout << loopNode->Data << ", ";
		loopNode = loopNode->Next;
	}

	std::cout << "}" << std::endl;
}

template <typename Type>
typename LinkedList<Type>::Node* LinkedList<Type>::GetPosPtr(uint32 pos, Node** prev)
{
	Node* curNode = mFront;

	for (uint32 index = 0; index < pos; ++index)
	{
		if (curNode == nullptr)
		{
			return nullptr;
		}

		if (prev != nullptr)
		{
			*prev = curNode;
		}
		
		curNode = curNode->Next;
	}

	return curNode;
}
