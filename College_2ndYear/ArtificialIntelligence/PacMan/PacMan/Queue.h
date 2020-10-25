#pragma once

#include <cassert>

template<typename T>
class Queue final
{
public:
	explicit Queue();
	~Queue();

	void Enqueue(const T& InData);
	void Dequeue();
	void Clear();

	const T& GetData() const;
	bool IsEmpty() const;
	int GetSize() const;

private:
	void ReallocArray();

private:
	T* mDataArray;
	int mHead;
	int mTail;
	int mCapacity;

};

template<typename T>
Queue<T>::Queue()
	: mDataArray(nullptr)
	, mCapacity(10)
	, mHead(0)
	, mTail(0)
{
	mDataArray = new T[mCapacity];

	for (int i = 0; i < mCapacity; ++i)
	{
		mDataArray[i] = T();
	}
}

template<typename T>
Queue<T>::~Queue()
{
	if (mDataArray != nullptr)
	{
		delete[] mDataArray;
		mDataArray = nullptr;
	}
}

template<typename T>
void Queue<T>::Enqueue(const T& InData)
{
	if (mTail - mHead >= mCapacity)
	{
		ReallocArray();
	}

	mDataArray[mTail % mCapacity] = InData;
	++mTail;
}

template<typename T>
void Queue<T>::Dequeue()
{
	if (IsEmpty() == true)
	{
		return;
	}

	mDataArray[mHead % mCapacity] = T();
	++mHead;
}

template <typename T>
void Queue<T>::Clear()
{
	while (!IsEmpty())
	{
	    Dequeue();
	}
}

template<typename T>
const T& Queue<T>::GetData() const
{
	assert(!IsEmpty());
	return mDataArray[mHead % mCapacity];
}

template<typename T>
inline bool Queue<T>::IsEmpty() const
{
	return mHead == mTail;
}

template<typename T>
int Queue<T>::GetSize() const
{
	return mTail - mHead;
}

template<typename T>
void Queue<T>::ReallocArray()
{
	assert(!IsEmpty());

	T* NewArray = new T[mCapacity * 2];
	for (int i = 0; i < mCapacity * 2; ++i)
	{
		NewArray[i] = T();
	}

	for (int i = mHead; i < mTail; ++i)
	{
		NewArray[i % (mCapacity * 2)] = mDataArray[i % mCapacity];
	}

	delete[] mDataArray;
	mDataArray = NewArray;
	mCapacity *= 2;
}
