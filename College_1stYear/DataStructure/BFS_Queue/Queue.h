#pragma once

#include <exception>

template<typename T>
class Queue final
{
public:

	Queue();
	~Queue();

	void Enqueue(const T& InData);
	void Dequeue();
	const T& GetData() const;
	bool IsEmpty() const;
	int GetSize() const;

private:

	void ReallocArray();

private:

	T* DataArray;
	int Head;
	int Tail;
	int Capacity;

};

template<typename T>
Queue<T>::Queue()
	: DataArray(nullptr)
	, Capacity(10)
	, Head(0)
	, Tail(0)
{
	DataArray = new T[Capacity];

	for (int i = 0; i < Capacity; ++i)
	{
		DataArray[i] = T();
	}
}

template<typename T>
Queue<T>::~Queue()
{
	if (DataArray != nullptr)
	{
		delete[] DataArray;
		DataArray = nullptr;
	}
}

template<typename T>
void Queue<T>::Enqueue(const T& InData)
{
	if (Tail - Head >= Capacity)
	{
		ReallocArray();
	}

	DataArray[Tail % Capacity] = InData;
	++Tail;
}

template<typename T>
void Queue<T>::Dequeue()
{
	if (IsEmpty() == true)
	{
		return;
	}

	DataArray[Head % Capacity] = T();
	++Head;
}

template<typename T>
const T& Queue<T>::GetData() const
{
	if (IsEmpty() == true)
	{
		throw std::exception("This array is empty!");
	}

	return DataArray[Head % Capacity];
}

template<typename T>
inline bool Queue<T>::IsEmpty() const
{
	return Head == Tail;
}

template<typename T>
int Queue<T>::GetSize() const
{
	return Tail - Head;
}

template<typename T>
void Queue<T>::ReallocArray()
{
	if (IsEmpty() == true)
	{
		throw std::exception("This array is empty!");
	}

	T* NewArray = new T[Capacity * 2];
	for (int i = 0; i < Capacity * 2; ++i)
	{
		NewArray[i] = T();
	}

	for (int i = Head; i < Tail; ++i)
	{
		NewArray[i % (Capacity * 2)] = DataArray[i % Capacity];
	}

	delete[] DataArray;
	DataArray = NewArray;
	Capacity *= 2;
}
