#include <iostream>
#include <memory>
#include "TestUniquePointer.h"

int main()
{
	Test::ExecUniquePointer();
	std::unique_ptr<int> Para = std::make_unique<int>(-50);

	if (Test::UniquePointerParameter(Para) == true)
	{
		std::cout << "ISTRUE" << std::endl;
	}
	else
	{
		std::cout << "ISFALSE" << std::endl;
	}
	return 0;
}