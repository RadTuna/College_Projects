
#include <iostream>
#include <cassert>

#include "LinkedList.h"

LinkedList::~LinkedList()
{
	if (Length == 0 || Head == nullptr)
	{
		return;
	}

	for (int i = Length - 1; i >= 0; --i)
	{
		Node* PendingKill = Head;

		for (int j = 0; j < i; ++j)
		{
			PendingKill = PendingKill->Next;
		}

		assert(PendingKill);
		delete PendingKill;
		PendingKill = nullptr;
	}
}

void LinkedList::Insert(int Value)
{
	Node* NewNode = new Node;
	NewNode->Data = Value;

	if (Head == nullptr)
	{
		Head = NewNode;
		NewNode->Next = nullptr;
	}
	else
	{
		NewNode->Next = Head;
		Head = NewNode;
	}

	++Length;
}

void LinkedList::Delete(int Value)
{
	Node* PrevPtr = nullptr;
	Node* CurrentPtr = Head;
	bool IsFound = false;
	for (int i = 0; i < Length; ++i)
	{
		if (CurrentPtr->Data == Value)
		{
			IsFound = true;
			break;
		}

		PrevPtr = CurrentPtr;
		CurrentPtr = CurrentPtr->Next;
	}

	if (IsFound == false)
	{
		std::cout << "The value could not be found." << std::endl;
		return;
	}

	if (PrevPtr == nullptr)
	{
		Node* PendingKill = Head;
		Head = Head->Next;
		delete PendingKill;
		PendingKill = nullptr;
	}

	assert(PrevPtr);
	assert(CurrentPtr);
	PrevPtr->Next = CurrentPtr->Next;
	delete CurrentPtr;
	CurrentPtr = nullptr;

	--Length;
}

void LinkedList::Print()
{
	Node* CurrentNode = Head;
	for (int i = 0; i < Length; ++i)
	{
		std::cout << CurrentNode->Data << ", ";

		CurrentNode = CurrentNode->Next;
	}

	std::cout << std::endl;
}
