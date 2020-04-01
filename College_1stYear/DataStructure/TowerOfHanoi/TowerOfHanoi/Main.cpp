#include <iostream>
#include "TowerOfHanoi.h"

using namespace std;

int main()
{
	int Count = 0;

	cout << "하노이의 탑 블럭 수를 입력: ";
	cin >> Count;

	cout << endl << "1은 첫번째 기둥, 2는 두번째 기둥, 3은 세번째 기둥입니다." << endl;
	cout << endl << "총 이동 횟수 : " << TowerOfHanoi(Hanoi::StartPoint, Hanoi::EndPoint, Hanoi::MidPoint, Count) << endl;
}