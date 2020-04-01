#include <iostream>
#include "Dog.h"


int main()
{
	Dog* Obj1 = new Dog("happy", 3, 4);
	Dog* Obj2 = new Dog(*Obj1);

	Obj1->ShowData();
	Obj2->ShowData();

	return 0;
}