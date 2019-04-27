#pragma once

class LinkedList
{
public:
	struct Node
	{
		int Value;
		Node* Next;
	};

	LinkedList(int Value);
	~LinkedList();

	void PushBack(int Value);
	int PopFront();
	void Print() const;

private:
	Node* Head;
	Node* Tail;
	int Size;
};
