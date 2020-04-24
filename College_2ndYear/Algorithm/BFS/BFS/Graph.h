#pragma once

#include <string>

#include "Queue.h"

template<typename Type>
class Graph
{
public:

	Graph(Type* nodeData, int* nodeConnections, int size);
	~Graph();


	bool FindPath(int** outArray, int* outSize, int start, int end);
	
	void SetMarker(int index);
	void ClearMarker();
	
	void GetNodeData(Type* outData, int index) const;
	bool IsConnected(int base, int target) const;
	bool IsMarked(int index) const;
	
private:

	int mNodeNum;
	Type* mNodeData = nullptr;
	bool* mNodeMarkers = nullptr;
	int** mNodeConnections = nullptr;
	int* mParentNodes = nullptr;
	
};

template <typename Type>
Graph<Type>::Graph(Type* nodeData, int* nodeConnections, int size)
{
	mNodeNum = size;
	
	mNodeData = new Type[size];
	const int dataSize = sizeof(Type) * size;
	memcpy(mNodeData, nodeData, dataSize);

	mNodeConnections = new int* [size];
	for (int index = 0; index < size; ++index)
	{
		mNodeConnections[index] = new int[size];
		for (int innerIndex = 0; innerIndex < size; ++innerIndex)
		{
			mNodeConnections[index][innerIndex] = nodeConnections[index * size + innerIndex];
		}
	}

	mNodeMarkers = new bool[size];
	const int markerSize = sizeof(bool) * size;
	memset(mNodeMarkers, 0, markerSize);

	mParentNodes = new int[size];
	const int parentNodeSize = sizeof(int) * size;
	memset(mParentNodes, 0, parentNodeSize);
}

template <typename Type>
Graph<Type>::~Graph()
{
	if (mParentNodes)
	{
		delete[] mParentNodes;
		mParentNodes = nullptr;
	}
	
	if (mNodeMarkers)
	{
		delete[] mNodeMarkers;
		mNodeMarkers = nullptr;
	}

	if (mNodeConnections)
	{
		for (int index = 0; index < mNodeNum; ++index)
		{
			delete[] mNodeConnections[index];
			mNodeConnections[index] = nullptr;
		}

		delete[] mNodeConnections;
		mNodeConnections = nullptr;
	}

	if (mNodeData)
	{
		delete[] mNodeData;
		mNodeData = nullptr;
	}
}

template <typename Type>
bool Graph<Type>::FindPath(int** outArray, int* outSize, int start, int end)
{
	if (start < 0 || start >= mNodeNum ||
		end < 0 || end >= mNodeNum)
	{
		return false;
	}

	Queue<int> queue;
	
	queue.EnQueue(start);
	mParentNodes[start] = start;
	SetMarker(start);

	bool bIsFind = false;
	for (;!bIsFind;)
	{
		const int frontIndex = queue.DeQueue();
		for (int index = 0; index < mNodeNum; ++index)
		{
			if (IsConnected(frontIndex, index) &&
				!IsMarked(index) && frontIndex != index)
			{
				SetMarker(index);
				queue.EnQueue(index);
				mParentNodes[index] = frontIndex;

				if (index == end)
				{
					bIsFind = true;
					break;
				}
			}
		}
	}

	if (bIsFind)
	{
		int pathCount = 0;
		int targetIndex = end;
		for (;targetIndex != mParentNodes[targetIndex];)
		{
			targetIndex = mParentNodes[targetIndex];
			++pathCount;
		}

		++pathCount;
		*outSize = pathCount;
		*outArray = new int[pathCount];

		targetIndex = end;
		for (int index = pathCount - 1; index >= 0; --index)
		{
			(*outArray)[index] = targetIndex;
			targetIndex = mParentNodes[targetIndex];
		}
	}

	return bIsFind;
}

template <typename Type>
void Graph<Type>::SetMarker(int index)
{
	if (index < 0 || index >= mNodeNum || !mNodeMarkers)
	{
		return;
	}
	
	mNodeMarkers[index] = true;
}

template <typename Type>
void Graph<Type>::ClearMarker()
{
	const int markerSize = sizeof(bool) * mNodeNum;
	memset(mNodeMarkers, 0, markerSize);
}

template <typename Type>
void Graph<Type>::GetNodeData(Type* outData, int index) const
{
	if (index < 0 || index >= mNodeNum || !mNodeData)
	{
		outData = nullptr;
		return;
	}

	*outData = mNodeData[index];
	return;
}

template <typename Type>
bool Graph<Type>::IsConnected(int base, int target) const
{
	if (base < 0 || base >= mNodeNum || 
		target < 0 || target >= mNodeNum ||
		base == target || !mNodeConnections)
	{
		return false;
	}

	return mNodeConnections[base][target] < 1000 ? true : false;
}

template <typename Type>
bool Graph<Type>::IsMarked(int index) const
{
	if (index < 0 || index >= mNodeNum || !mNodeMarkers)
	{
		return false;
	}

	return mNodeMarkers[index];
}
