#include "Gun.h"
#include "Police.h"

int main()
{
	Police pman1(5, 3);
	Police pman2 = pman1;
	pman2.PutHandcuff();
	pman2.ShowHandcuff();
	pman2.Shot();
	pman2.ShowBullet();

	Police pman3(2, 4);
	pman3 = pman1;
	pman3.PutHandcuff();
	pman3.ShowHandcuff();
	pman3.Shot();
	pman3.ShowBullet();

	return 0;
}