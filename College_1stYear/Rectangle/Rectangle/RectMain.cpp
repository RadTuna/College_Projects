#include <iostream>
#include "Rectangle.h"

int main()
{
	float FirstX = 0.0f;
	float FirstY = 0.0f;
	std::cout << "ù��° ������ ��ǥ�� �Է��ϼ���" << std::endl;
	std::cin >> FirstX >> FirstY;

	float SecondX = 0.0f;
	float SecondY = 0.0f;
	std::cout << "�ι�° ������ ��ǥ�� �Է��ϼ���" << std::endl;
	std::cin >> SecondX >> SecondY;

	Rectangle Rect(FirstX, FirstY, SecondX, SecondY);

	std::cout << std::endl;
	std::cout << "�簢�� ���� : " << Rect.GetRectArea() << std::endl;

	return 0;
}