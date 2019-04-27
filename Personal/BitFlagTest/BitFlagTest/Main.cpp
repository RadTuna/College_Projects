#include <iostream>
#include "BitFlag.h"

using namespace std;

int main()
{
	BitFlag Field;

	cout << Field.GetDirectData() << endl;

	Field.SetBitFlag(2, true);
	if (Field.GetBitFlag(2))
	{
		cout << "True" << endl;
	}
	else
	{
		cout << "False" << endl;
	}

	cout << Field.GetDirectData() << endl;

	Field.SetBitFlag(2, false);
	if (Field.GetBitFlag(2))
	{
		cout << "True" << endl;
	}
	else
	{
		cout << "False" << endl;
	}

	cout << Field.GetDirectData() << endl;

	Field.SetBitFlag(5, false);
	if (Field.GetBitFlag(5))
	{
		cout << "True" << endl;
	}
	else
	{
		cout << "False" << endl;
	}

	cout << Field.GetDirectData() << endl;

	return 0;
}