#include <iostream>
#include "Point.h"
#include "AirConditioner.h"
#include "Calculator.h"

int main()
{
	// ���� 1�� ����.
	std::cout << std::endl << "***** ���� 1�� ���� *****" << std::endl << std::endl;
	Point MyPoint;

	MyPoint.SetPos(20.0f, 35.0f);
	MyPoint.Show();
	std::cout << std::endl << "***** ���� 1�� �� *****" << std::endl << std::endl;
	// ���� 1�� ��.


	// ���� 2�� ����.
	std::cout << std::endl << "***** ���� 2�� ���� *****" << std::endl << std::endl;
	AirConditioner MyAirCon;

	MyAirCon.TogglePower();
	MyAirCon.SetTemp(25.0f);
	MyAirCon.TempUpOne();

	std::cout << "CurrentTemp : " << MyAirCon.GetTemp() << std::endl;

	MyAirCon.TempDownOne();
	MyAirCon.TempDownOne();
	MyAirCon.PrintTemp();
	MyAirCon.TogglePower();
	MyAirCon.PrintTemp();
	std::cout << std::endl << "***** ���� 2�� �� *****" << std::endl << std::endl;
	// ���� 2�� ��.


	// ���� 3�� ����.
	std::cout << std::endl << "***** ���� 3�� ���� *****" << std::endl << std::endl;
	Calculator MyCalculator;

	float aNum;
	float bNum;
	float Result;
	char Operator;

	std::cout << "������ ���ϴ� �ΰ��� ���ڸ� �Է��ϼ���." << std::endl;
	std::cin >> aNum >> bNum;

	std::cout << "������ ���ϴ� ��ȣ�� �Է��ϼ���" << std::endl;
	std::cin >> Operator;

	switch (Operator)
	{
	case '+':
		Result = MyCalculator.Add(aNum, bNum);
		break;
	case '-':
		Result = MyCalculator.Sub(aNum, bNum);
		break;
	case '*':
		Result = MyCalculator.Mul(aNum, bNum);
		break;
	case '/':
		Result = MyCalculator.Div(aNum, bNum);
		break;
	default:
		std::cout << "������ �Է��� �߸��Ǿ����ϴ�. �����մϴ�." << std::endl;
		return 0;
	}

	std::cout << "Calculate result : " << Result << std::endl;

	std::cout << std::endl << "***** ���� 3�� �� *****" << std::endl << std::endl;
	// ���� 3�� ��.


	return 0;
}