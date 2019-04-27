#include <iostream>
#include "LinkedList.h"

using namespace std;

LinkedList::LinkedList(int Value)
{
	Node* List = new Node();

	List->Value = Value;
	Head = Tail = List;
	Size = 0;
}

LinkedList::~LinkedList()
{
	while (Head != nullptr)
	{
		cout << "Destoryer : " << Head->Value << endl;
		Node* TempPtr = Head;
		Head = Head->Next;
		delete TempPtr;
	}
}

void LinkedList::PushBack(int Value)
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

int LinkedList::PopFront()
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

void LinkedList::Print() const
{
	Node* TempPtr = Head;
	while (TempPtr != nullptr)
	{
		cout << " " << TempPtr->Value << endl;
		TempPtr = TempPtr->Next;
	}
}