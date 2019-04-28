#include <iostream>
#include "SingleLinkedList.h"

using namespace std;

SingleLinkedList::SingleLinkedList(int Value)
{
	Node* List = new Node();

	List->Value = Value;
	Head = Tail = List;
	Size = 0;
}

SingleLinkedList::~SingleLinkedList()
{
	while (Head != nullptr)
	{
		cout << "Destoryer : " << Head->Value << endl;
		Node* TempPtr = Head;
		Head = Head->Next;
		delete TempPtr;
	}
}

void SingleLinkedList::PushBack(int Value)
{
	Node* List = new Node();

	List->Value = Value;
	if (Head == nullptr)
	{
		Head = Tail = List;
	}
	else
	{
		Tail->Next = List;
		Tail = List;
	}
	Size++;
}

int SingleLinkedList::PopFront()
{
	if (Head == nullptr)
	{
		return 0;
	}

	int ReValue = Head->Value;
	Node* DelTemp = Head;

	Head = Head->Next;
	delete DelTemp;
	Size--;

	return ReValue;
}

void SingleLinkedList::Print() const
{
	Node* TempPtr = Head;
	while (TempPtr != nullptr)
	{
		cout << " " << TempPtr->Value << endl;
		TempPtr = TempPtr->Next;
	}
}