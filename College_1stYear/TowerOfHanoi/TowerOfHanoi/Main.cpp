#include <iostream>
#include "TowerOfHanoi.h"

using namespace std;

int main()
{
	int Count = 0;

	cout << "�ϳ����� ž �� ���� �Է�: ";
	cin >> Count;

	cout << endl << "1�� ù��° ���, 2�� �ι�° ���, 3�� ����° ����Դϴ�." << endl;
	cout << endl << "�� �̵� Ƚ�� : " << TowerOfHanoi(Hanoi::StartPoint, Hanoi::EndPoint, Hanoi::MidPoint, Count) << endl;
}