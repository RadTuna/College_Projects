#pragma once

template<typename T>
class Stack
{
private:

	struct Node
	{
		T Data;
		Node* Next;
	};

public:

	Stack();
	~Stack();

	void Push(const T& InData);
	void Pop();
	const T& Get() const;
	bool IsEmpty() const;
	unsigned int GetLength() const;

private:

	Node* Head;
	unsigned int Length = 0;

};

template<typename T>
Stack<T>::Stack()
{
	Head = nullptr;
}

template<typename T>
Stack<T>::~Stack()
{
	for (; IsEmpty() == false;)
	{
		Node* Temp = Head->Next;
		delete Head;
		Head = Temp;
	}
}

template<typename T>
void Stack<T>::Push(const T& InData)
{
	Node* NewNode = new Node;
	NewNode->Data = InData;
	NewNode->Next = Head;
	Head = NewNode;
	Length++;
}

template<typename T>
void Stack<T>::Pop()
{
	if (Head == nullptr)
	{
		return;
	}

	Node* Temp = Head->Next;
	delete Head;
	Head = Temp;
	Length--;
}

template<typename T>
const T& Stack<T>::Get() const
{
	return Head->Data;
}

template<typename T>
bool Stack<T>::IsEmpty() const
{
	return Head == nullptr;
}

template<typename T>
inline unsigned int Stack<T>::GetLength() const
{
	return Length;
}
