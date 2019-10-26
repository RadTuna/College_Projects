#include "Car.h"
#include "Plane.h"

int main()
{
	Vehicle* pv[2];
	Car c(8604, 25.5, 80);
	pv[0] = &c;
	Plane pl(940, 500);
	pv[1] = &pl;

	pv[0]->Show();
	pv[1]->Show();

	return 0;
}