
#include <iostream>
#include "Acceleration.h"

using namespace std;

int main()
{
	float Dist = Acceleration::GetDistance(20.0f, 3.0f, 10.0f);

	cout << Dist;
}