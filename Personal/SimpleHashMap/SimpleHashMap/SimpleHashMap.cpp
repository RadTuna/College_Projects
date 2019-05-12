#include <string>
#include <algorithm>
#include <iostream>
#include "SimpleData.h"
#include "SimpleHashMap.h"

void SimpleHashMap::Add(const char* Key, unsigned long Hash, int Value)
{
	shared_ptr<Node> NewNode = make_shared<Node>(Key, Value);

	int Index = Hash % MAX_SIZE;

	if (mArray[Index] == nullptr)
	{
		mArray[Index] = NewNode;
		return;
	}

	shared_ptr<Node> Current = mArray[Index];
	if (Current->GetKey() == Key)
	{
		NewNode->SetNext(Current->GetNext());
		mArray[Index] = NewNode;
		return;
	}

	while (Current->GetNext() != nullptr)
	{
		if (Current->GetNext()->GetKey() == Key)
		{
			NewNode->SetNext(Current->GetNext()->GetNext());
			Current->SetNext(NewNode);
			return;
		}
	}
	Current->SetNext(NewNode);
}

int SimpleHashMap::Get(const char* Key, unsigned long Hash) const
{
	int Index = Hash % MAX_SIZE;

	if (mArray[Index] == nullptr)
	{
		return -1;
	}

	shared_ptr<Node> Current = mArray[Index];

	while (Current != nullptr)
	{
		if (Current->GetKey() == Key)
		{
			return Current->GetDate();
		}
		Current = Current->GetNext();
	}
	return -1;
}

void SimpleHashMap::Print() const
{
	for (int i = 0; i < MAX_SIZE; ++i)
	{
		if (mArray[i] != nullptr)
		{
			shared_ptr<Node> Current = mArray[i];

			while (Current != nullptr)
			{
				cout << "Key: " << Current->GetKey() << ", Value: " << Current->GetDate() << endl;
				Current = Current->GetNext();
			}
		}
	}
}