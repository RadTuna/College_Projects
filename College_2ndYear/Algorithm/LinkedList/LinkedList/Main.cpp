#include <iostream>

#include "LinkedList.h"

int main()
{
	LinkedList<int> TestList;

	TestList.Insert(0, 25);
	TestList.Insert(1, 50);
	TestList.Insert(2, 75);
	TestList.Print();

	TestList.Remove(1);
	TestList.Print();


	TestList.Insert(0, 0);
	TestList.Insert(1, -58);
	TestList.Print();

	TestList.Remove(3);
	TestList.Print();
}