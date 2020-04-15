#pragma once

#include "TypeDef.h"

template<typename Type>
struct Node
{
	Type Data;
	Node* Next;
};

template<typename Type>
class Stack
{
public:

	Stack() = default;
	~Stack() = default;


	void Push(const Type& data);
	void Pop();
	bool Get(Type* outData) const;
	

	bool IsEmpty() const;

private:

	Node<Type>* mFront = nullptr;
	uint32 mSize = 0;
	
};

template <typename Type>
void Stack<Type>::Push(const Type& data)
{
	Node<Type>* newNode = new Node<Type>();
	newNode->Data = data;
	newNode->Next = mFront;

	mFront = newNode;
	++mSize;
}

template <typename Type>
void Stack<Type>::Pop()
{
	if (IsEmpty())
	{
		return;
	}

	Node<Type>* pendingData = mFront;
	mFront = mFront->Next;
	delete pendingData;
}

template <typename Type>
bool Stack<Type>::Get(Type* outData) const
{
	if (IsEmpty())
	{
		return false;
	}
	
	*outData = mFront->Data;
	return true;
}

template <typename Type>
bool Stack<Type>::IsEmpty() const
{
	return mFront == nullptr;
}
