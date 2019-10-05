#include <iostream>

void testptr(int* test);
void testref(int& test);

int main()
{
	int i = 10;

	int j = i;

	int& ref = i;

	int* ptr = &i;

	j = ref;

	testptr(&i);
	testref(i);

	return 0;
}

void testptr(int* test)
{
	*test += 1;
	return;
}

void testref(int& test)
{
	test += 1;
	return;
}