#pragma once

#include <memory>
#include "Entity.h"

class SharedList
{
public:

	explicit SharedList();
	~SharedList();

	bool Insert(int Index, const Entity& InData);
	bool Remove(int Index);
	bool Retrieve(int Index, Entity& OutData);
	Entity* RetrievePtr(int Index);
	void Destroy();
	bool IsEmpty();
	int GetLength();

private:

	struct Node
	{
		Entity Data;
		std::shared_ptr<Node> Next;

		Node(const Entity& InData) : Data(InData), Next(nullptr) {}
	};

	std::shared_ptr<Node> Head;
	int Length;

};
