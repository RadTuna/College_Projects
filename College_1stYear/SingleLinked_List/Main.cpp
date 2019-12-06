
#include "LinkedList.h"

int main()
{
	LinkedList MyList;

	for (int i = 1; i <= 10; i < ++i)
	{
		MyList.Insert(i * i);
	}

	MyList.Print();

	MyList.Delete(64);
	MyList.Delete(16);

	MyList.Print();

	return 0;
}