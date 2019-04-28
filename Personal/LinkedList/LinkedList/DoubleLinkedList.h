#pragma once

class DoubleLinkedList
{
public:
	DoubleLinkedList(int Value);
	~DoubleLinkedList();

	void PushBack(int Value);
	void PushFront(int Value);
	int PopBack();
	int PopFront();
	void Insert(int Value, int Index, bool IsStartFront = true);
	void Delete(int Index, bool IsStartFront = true);

	void Print();

private:
	struct Node
	{
		int Value;
		Node* next;
		Node* prev;
	};

	int ListSize;
	Node* Head;
	Node* Tail;
};