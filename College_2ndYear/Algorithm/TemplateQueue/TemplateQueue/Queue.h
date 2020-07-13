#pragma once

#include <cassert>
#include <iostream>
#include <memory>

constexpr unsigned int QUEUE_DEFAULT_CAPACITY = 32;

template<typename Type>
class Queue final
{
public:

	explicit Queue() noexcept;
	explicit Queue(unsigned int reserve) noexcept;
	~Queue() noexcept;

	void Enqueue(const Type& inData);
	void Dequeue(Type* outData);
	void Draw();
	
	const Type* GetData() const { return mData; }
	[[nodiscard]] unsigned int GetSize() const { return mSize; }
	[[nodiscard]] unsigned int GetCapacity() const { return mCapacity; }
	[[nodiscard]] bool IsEmpty() const { return mSize <= 0; }

private:

	void ReAllocation(unsigned int CapaMultiplier = 2);
	
private:

	Type* mData;
	unsigned int mFront;
	unsigned int mRear;
	unsigned int mSize;
	unsigned int mCapacity;
	
};

#include "Queue.inl"

