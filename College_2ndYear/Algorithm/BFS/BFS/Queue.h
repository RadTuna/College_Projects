#pragma once 


template<typename Type>
struct Node
{
    Type Data;
    Node* Next;
};


template<typename Type>
class Queue
{
public:

    Queue(void) :mFront(nullptr), mTail(nullptr), mSize(0) {}
    ~Queue(void) {}

    void EnQueue(int item);
    int DeQueue();
	
    int GetSize() { return mSize; }

    int GetFront() {
        if (mFront)
        {
            return mFront->Data;
        }
        return -1;
    }

private:

    Node<Type>* mFront;
    Node<Type>* mTail;
    int mSize;
	
};

template<typename Type>
void Queue<Type>::EnQueue(int item)
{
    Node<Type>* temp = new Node<Type>;
    temp->Data = item;
    temp->Next = nullptr;

    if (!mFront) 
    {              
        mTail = temp;
        mFront = temp;
    }
    else 
    {
        mTail->Next = temp;
        mTail = temp;
    }

    mSize++;
}


template<typename Type>
int Queue<Type>::DeQueue()
{
    Node<Type>* temp = nullptr;
    int retVal = -1;

    if (mSize == 0)
    {
        return -1;
    }

    retVal = mFront->Data;
    temp = mFront;

    mFront = mFront->Next;

    delete temp;
    mSize--;

    return retVal;
}
