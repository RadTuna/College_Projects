#include "CSet.h"
#include <iostream>

int main()
{
	// 집합배열의 최대크기는 10;

	//CSet MySet1;
	//MySet1.Insert(88);
	//MySet1.Insert(33);
	//MySet1.Insert(99);
	//MySet1.Insert(77);

	//CSet MySet2;
	//MySet2.Insert(10);
	//MySet2.Insert(20);
	//MySet2.Insert(30);

	//MySet1.Print();
	//MySet2.Print();

	//MySet1.Remove(77);
	//std::cout << "s1-(77)=";
	//MySet1.Print();

	//if (MySet2.Member(20))
	//{
	//	std::cout << "20 is in Set2" << std::endl;
	//}

	//if (MySet1.Equal(&MySet2) == false)
	//{
	//	std::cout << "Set1 != Set2" << std::endl;
	//}

	//MySet1.Print();

	//CSet MySet3;
	//MySet1.Union(&MySet2, &MySet3);
	//std::cout << "Set3 = Set1 U Set2 = ";
	//MySet3.Print();

	//MySet3.Empty();

	//MySet1.Intersection(&MySet2, &MySet3);
	//std::cout << "Set3 = Set1 N Set2 = ";
	//MySet3.Print();

	//***************************************

	CSet MySet1;
	// Insert - operator()
	MySet1(88);
	MySet1(33);
	MySet1(99);
	MySet1(77);

	CSet MySet2;
	MySet2(10);
	MySet2(20);
	MySet2(30);

	MySet1.Print();
	MySet2.Print();

	// Remove - operator-
	MySet1 - 77;
	std::cout << "s1-(77)=";
	MySet1.Print();

	// Member - operator[]
	if (MySet2[20])
	{
		std::cout << "20 is in Set2" << std::endl;
	}

	// Equal - operator==
	if (MySet1 == MySet2)
	{
		std::cout << "Set1 != Set2" << std::endl;
	}

	MySet1.Print();

	CSet MySet3;
	// Union - operator+
	MySet3 = MySet1 + MySet2;
	std::cout << "Set3 = Set1 U Set2 = ";
	MySet3.Print();

	MySet3.Empty();

	// Intersection - operator*
	MySet3 = MySet1 * MySet2;
	std::cout << "Set3 = Set1 N Set2 = ";
	MySet3.Print();

	return 0;
}