#include <iostream>
#include "DoubleLinkedList.h"

DoubleLinkedList::DoubleLinkedList(int Value)
	: ListSize(1)
{
	Node* List = new Node();

	List->Value = Value;
	Head = Tail = List;
}

DoubleLinkedList::~DoubleLinkedList()
{
	if (Head != nullptr)
	{
		Node* Temp = Head;
		while (Temp != nullptr)
		{
			Temp = Head->next;
			delete Head;
			Head = Temp;
		}
	}
	else if (Tail != nullptr)
	{
		Node* Temp = Tail;
		while (Temp != nullptr)
		{
			Temp = Head->next;
			delete Tail;
			Temp = Tail;
		}
	}
}

void DoubleLinkedList::PushBack(int Value)
{
	if (Tail == nullptr)
	{
		Node* List = new Node();

		List->Value = Value;
		Head = Tail = List;
		ListSize = 1;
	}
	else
	{
		Node* List = new Node();

		List->Value = Value;
		List->prev = Tail;
		Tail->next = List;
		Tail = List;
		ListSize++;
	}
}

void DoubleLinkedList::PushFront(int Value)
{
	if (Tail == nullptr)
	{
		Node* List = new Node();

		List->Value = Value;
		Head = Tail = List;
		ListSize = 1;
	}
	else
	{
		Node* List = new Node();

		List->Value = Value;
		List->next = Head;
		Head->prev = List;
		Head = List;
		ListSize++;
	}
}

int DoubleLinkedList::PopBack()
{
	if (Tail == nullptr)
	{
		return 0;
	}
	int ResultValue = Tail->Value;
	Node* Temp = Tail;

	Tail->prev->next = nullptr;
	Tail = Tail->prev;
	delete Temp;
	ListSize--;

	return ResultValue;
}

int DoubleLinkedList::PopFront()
{
	if (Head == nullptr)
	{
		return 0;
	}
	int ResultValue = Head->Value;
	Node* Temp = Head;

	Head->next->prev = nullptr;
	Head = Head->next;
	delete Temp;
	ListSize--;

	return ResultValue;
}

void DoubleLinkedList::Insert(int Value, int Index, bool IsStartFront)
{
	if (Index < 0 || Index > ListSize - 1)
	{
		return;
	}

	if (IsStartFront == true)
	{
		Node* Temp = Head;
		for (int i = 0; i < Index - 1; ++i)
		{
			Temp = Temp->next;
		}
		Node* List = new Node();

		List->Value = Value;
		List->prev = Temp;
		List->next = Temp->next;
		Temp->next->prev = List;
		Temp->next = List;
	}
	else
	{
		Node* Temp = Tail;
		for (int i = 0; i < Index - 1; ++i)
		{
			Temp = Temp->prev;
		}
		Node* List = new Node();

		List->Value = Value;
		List->next = Temp;
		List->prev = Temp->prev;
		Temp->prev->next = List;
		Temp->prev = List;
	}
}

void DoubleLinkedList::Delete(int Index, bool IsStartFront)
{
	if (Index < 0 || Index > ListSize - 1)
	{
		return;
	}

	Node* Temp;
	if (IsStartFront == true)
	{
		Temp = Head;
		for (int i = 0; i < Index; ++i)
		{
			Temp = Temp->next;
		}
	}
	else
	{
		Temp = Tail;
		for (int i = 0; i < Index; ++i)
		{
			Temp = Temp->prev;
		}
	}

	if (Temp->prev == nullptr)
	{
		Temp->next->prev = nullptr;
		delete Temp;
	}
	else if (Temp->next == nullptr)
	{
		Temp->prev->next = nullptr;
		delete Temp;
	}
	else
	{
		Temp->prev->next = Temp->next;
		Temp->next->prev = Temp->prev;
		delete Temp;
	}
}

void DoubleLinkedList::Print()
{
	if (Head == nullptr)
	{
		std::cout << "값이 존재하지 않습니다." << std::endl;
	}

	Node* Temp = Head;
	while (Temp != nullptr)
	{
		std::cout << "Value : " << Temp->Value << std::endl;

		Temp = Temp->next;
	}
}

