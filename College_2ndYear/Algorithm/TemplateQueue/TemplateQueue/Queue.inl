
#include "Queue.h"

template <typename Type>
Queue<Type>::Queue() noexcept
	: Queue(QUEUE_DEFAULT_CAPACITY)
{
}

template <typename Type>
Queue<Type>::Queue(unsigned int reserve) noexcept
	: mCapacity(reserve)
	, mSize(0)
	, mFront(0)
	, mRear(0)
{
	mData = new Type[reserve];
	memset(mData, 0, reserve * sizeof(Type));
}

template <typename Type>
Queue<Type>::~Queue() noexcept
{
	if (mData != nullptr)
	{
		delete[] mData;
		mData = nullptr;
	}
}

template <typename Type>
void Queue<Type>::Enqueue(const Type& inData)
{
	assert(mData != nullptr);
	if (mSize >= mCapacity)
	{
		ReAllocation();
	}
	
	mData[mRear % mCapacity] = inData;
	
	++mRear;
	++mSize;
}

template <typename Type>
void Queue<Type>::Dequeue(Type* outData)
{
	assert(mData != nullptr);
	if (mSize <= 0)
	{
		return;
	}

	*outData = mData[mFront % mCapacity];

	++mFront;
	--mSize;
}

template <typename Type>
void Queue<Type>::Draw()
{
	std::cout << "Queue : " << std::endl;
	for (unsigned int i = mFront; i <= mRear; ++i)
	{
		std::cout << mData[i % mCapacity] << " ";
	}
	std::cout << std::endl;
}

template <typename Type>
void Queue<Type>::ReAllocation(unsigned int CapaMultiplier)
{
	const unsigned int newCapacity = mCapacity * CapaMultiplier;
	Type* newData = new Type[newCapacity];
	memset(newData, 0, newCapacity * sizeof(Type));

	for (unsigned int i = mFront; i < mRear; ++i)
	{
		newData[i % newCapacity] = mData[i % mCapacity];
	}

	delete[] mData;
	mData = newData;
	mCapacity = newCapacity;
}

