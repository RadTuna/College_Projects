#include <iostream>
#include "Point.h"
#include "AirConditioner.h"
#include "Calculator.h"

int main()
{
	// 예제 1번 시작.
	std::cout << std::endl << "***** 예제 1번 시작 *****" << std::endl << std::endl;
	Point MyPoint;

	MyPoint.SetPos(20.0f, 35.0f);
	MyPoint.Show();
	std::cout << std::endl << "***** 예제 1번 끝 *****" << std::endl << std::endl;
	// 예제 1번 끝.


	// 예제 2번 시작.
	std::cout << std::endl << "***** 예제 2번 시작 *****" << std::endl << std::endl;
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
	std::cout << std::endl << "***** 예제 2번 끝 *****" << std::endl << std::endl;
	// 예제 2번 끝.


	// 예제 3번 시작.
	std::cout << std::endl << "***** 예제 3번 시작 *****" << std::endl << std::endl;
	Calculator MyCalculator;

	float aNum;
	float bNum;
	float Result;
	char Operator;

	std::cout << "연산을 원하는 두가지 숫자를 입력하세요." << std::endl;
	std::cin >> aNum >> bNum;

	std::cout << "연산을 원하는 기호를 입력하세요" << std::endl;
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
		std::cout << "연산자 입력이 잘못되었습니다. 종료합니다." << std::endl;
		return 0;
	}

	std::cout << "Calculate result : " << Result << std::endl;

	std::cout << std::endl << "***** 예제 3번 끝 *****" << std::endl << std::endl;
	// 예제 3번 끝.


	return 0;
}