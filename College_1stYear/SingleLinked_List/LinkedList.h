#pragma once

struct Node
{
	int Data;
	Node* Next;
};

class LinkedList
{
public:

	LinkedList() : Head(nullptr) {}
	~LinkedList();

	void Insert(int Value);
	void Delete(int Value);
	void Print();

private:

	Node* Head;
	int Length = 0;

public:

	inline int GetLength() const { return Length; }

};
