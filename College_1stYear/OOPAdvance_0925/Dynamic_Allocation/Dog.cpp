#include "Dog.h"
#include <string.h>
#include <iostream>


Dog::Dog()
{
	Name = nullptr;
}

Dog::Dog(const char* InName, int InAge, int InWeight)
	: Age(InAge),
	Weight(InWeight)
{
	Name = new char[strlen(InName) + 1];
	strcpy_s(Name, strlen(InName) + 1, InName);
}

Dog::Dog(const Dog& Other)
	: Age(Other.Age),
	Weight(Other.Weight)
{
	Name = new char[strlen(Other.Name) + 1];
	strcpy_s(Name, strlen(Other.Name) + 1, Other.Name);
}

Dog::~Dog()
{	
	if (Name != nullptr)
	{
		delete[] Name;
		Name = nullptr;
	}	
}

void Dog::ShowData()
{
	std::cout << "이름 : " << Name << std::endl;
	std::cout << "나이 : " << Age << std::endl;
	std::cout << "체중 : " << Weight << std::endl;
}

