#include <iostream>
#include "SingleLinkedList.h"
#include "DoubleLinkedList.h"
#define BLANK std::cout << std::endl

using namespace std;

int main()
{
	/*

	SingleLinkedList List(3);

	List.PushBack(5);
	List.PushBack(1);
	List.PushBack(15);
	List.PushBack(31);
	List.PushBack(24);

	List.Print();

	cout << "Pop : " << List.PopFront() << endl;
	cout << "Pop : " << List.PopFront() << endl;
	cout << "Pop : " << List.PopFront() << endl;
	cout << "Pop : " << List.PopFront() << endl;

	List.Print();

	*/

	DoubleLinkedList List(5);

	List.PushFront(10);
	List.PushBack(55);
	List.Print();

	BLANK;

	List.Insert(81, 2, true);
	List.Print();

	BLANK;

	List.PushFront(10);
	cout << "Pop : " << List.PopBack() << endl;
	cout << "Pop : " << List.PopFront() << endl;
	List.Print();

	BLANK;

	List.PushBack(99);
	List.PushFront(3);
	List.Delete(1, false);
	List.Print();
}