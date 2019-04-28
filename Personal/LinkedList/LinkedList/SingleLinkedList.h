#pragma once

class SingleLinkedList
{
public:
	struct Node
	{
		int Value;
		Node* Next;
	};

	SingleLinkedList(int Value);
	~SingleLinkedList();

	void PushBack(int Value);
	int PopFront();
	void Print() const;

private:
	Node* Head;
	Node* Tail;
	int Size;
};
