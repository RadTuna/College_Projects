#include "SharedList.h"



SharedList::SharedList()
	: Head(nullptr)
	, Length(0)
{
}

SharedList::~SharedList()
{
	Destroy();
}

bool SharedList::Insert(int Index, const Entity& InData)
{
	if (Index < 0 || Index > Length)
	{
		return false;
	}

	std::shared_ptr<Node> NewNode = std::make_shared<Node>(InData);

	if (Head == nullptr)
	{
		Head = NewNode;
		++Length;
		return true;
	}
	if (Index == 0)
	{
		std::shared_ptr<Node> TempNode = Head;
		Head = NewNode;
		Head->Next = TempNode;
		++Length;
		return true;
	}

	std::shared_ptr<Node> CurPoint = Head;
	for (int i = 0; i < Index - 1; ++i)
	{
		CurPoint = CurPoint->Next;
	}

	if (CurPoint->Next == nullptr)
	{
		CurPoint->Next = NewNode;
	}
	else
	{
		NewNode->Next = CurPoint->Next;
		CurPoint->Next = NewNode;
	}

	++Length;

	return true;
}

bool SharedList::Remove(int Index)
{
	if (Index < 0 || Index > Length)
	{
		return false;
	}
	if (IsEmpty() == true)
	{
		return false;
	}

	if (Index == 0)
	{
		Head = Head->Next;
		--Length;
		return true;
	}

	std::shared_ptr<Node> PrevPoint = Head;
	for (int i = 0; i < Index - 1; ++i)
	{
		PrevPoint = PrevPoint->Next;

		if (PrevPoint->Next == nullptr)
		{
			return false;
		}
	}

	// PrevPoint에 Next을 덧씌우면 PrevPoint 공유포인터 객체에 덮어씌우는 꼴이 되기에,
	// 아래와 같이 Next = Next->Next 꼴로 해야 정상적으로 덮어씌울 수 있음.
	PrevPoint->Next = PrevPoint->Next->Next;

	--Length;

	return true;
}

bool SharedList::Retrieve(int Index, Entity& OutData)
{
	if (Index < 0 || Index > Length)
	{
		return false;
	}

	std::shared_ptr<Node> CurPoint = Head;
	for (int i = 0; i < Index; ++i)
	{
		CurPoint = CurPoint->Next;

		if (CurPoint == nullptr)
		{
			return false;
		}
	}

	OutData = CurPoint->Data;

	return true;
}

Entity* SharedList::RetrievePtr(int Index)
{
	if (Index < 0 || Index > Length)
	{
		return nullptr;
	}
	if (Head == nullptr)
	{
		return nullptr;
	}

	std::shared_ptr<Node> CurPoint = Head;
	for (int i = 0; i < Index; ++i)
	{
		CurPoint = CurPoint->Next;

		if (CurPoint == nullptr)
		{
			return nullptr;
		}
	}

	return &CurPoint->Data;
}

void SharedList::Destroy()
{
	Head = nullptr;
}

bool SharedList::IsEmpty()
{
	return Head == nullptr;
}

int SharedList::GetLength()
{
	return Length;
}
