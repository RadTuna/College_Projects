#pragma once

class Dog
{
public:
	Dog();
	Dog(const char* InName, int InAge, int InWeight);
	Dog(const Dog& Other);
	~Dog();

	void ShowData();

private:

	char* Name;
	int Age;
	int Weight;

};
