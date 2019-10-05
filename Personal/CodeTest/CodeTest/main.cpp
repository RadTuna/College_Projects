#include <iostream>
#include "TimeCounter.h"

struct TestST
{
	float test[10000];
	int test2[10000];
};

void ValueFunc(TestST inV)
{
	return;
}

void RefFunc(TestST& inV)
{
	return;
}

void PtrFunc(TestST* inV)
{
	return;
}

int main()
{
	TimeCounter timer;

	TestST TestVal;
	const int loopCount = 1000;
	/*
	timer.Initialize();
	timer.Frame();
	for (int i = 0; i < loopCount; ++i)
	{
		ValueFunc(TestVal);
	}
	timer.Frame();
	std::cout << "Value : " << timer.GetDeltaTime() << std::endl;
	*/

	
	timer.Initialize();
	timer.Frame();
	for (int i = 0; i < loopCount; ++i)
	{
		PtrFunc(&TestVal);
	}
	timer.Frame();
	std::cout << "Ptr : " << timer.GetDeltaTime() << std::endl;
	
	/*
	timer.Initialize();
	timer.Frame();
	for (int i = 0; i < loopCount; ++i)
	{
		RefFunc(TestVal);
	}
	timer.Frame();
	std::cout << "Ref : " << timer.GetDeltaTime() << std::endl;
	*/

	return 0;
}