#include <iostream>
#include "LinkedList.h"

using namespace std;

int main()
{
	LinkedList List(3);

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
}