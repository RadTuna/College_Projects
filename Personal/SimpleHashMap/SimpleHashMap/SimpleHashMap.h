#pragma once

#include <memory>
#include "Node.h"
#include "SimpleData.h"

class SimpleHashMap
{
public:
	SimpleHashMap() = default;
	~SimpleHashMap() = default;

	void Add(const char* Key, unsigned long Hash, int Value);
	int Get(const char* Key, unsigned long Hash) const;
	void Print() const;

private:
	static constexpr int MAX_SIZE = 10;
	shared_ptr<Node> mArray[MAX_SIZE];
};