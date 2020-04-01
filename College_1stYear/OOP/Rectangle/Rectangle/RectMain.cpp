#include <iostream>
#include "Rectangle.h"

int main()
{
	float FirstX = 0.0f;
	float FirstY = 0.0f;
	std::cout << "첫번째 정점의 좌표를 입력하세요" << std::endl;
	std::cin >> FirstX >> FirstY;

	float SecondX = 0.0f;
	float SecondY = 0.0f;
	std::cout << "두번째 정점의 좌표를 입력하세요" << std::endl;
	std::cin >> SecondX >> SecondY;

	Rectangle Rect(FirstX, FirstY, SecondX, SecondY);

	std::cout << std::endl;
	std::cout << "사각형 면적 : " << Rect.GetRectArea() << std::endl;

	return 0;
}