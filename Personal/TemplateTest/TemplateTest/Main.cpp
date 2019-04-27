#include <iostream>
#include "MyArray.h"
#include "SimpleArray.h"
//#include "SimpleArray.cpp"

using namespace std;

/*
int main()
{
	MyArray<float> FScores;
	MyArray<int> IScores;

	FScores.Add(2.5f);
	FScores.Add(4.8f);
	FScores.Add(22.3f);
	FScores.Add(29.1f);

	FScores.GetArraySize();
	FScores.GetArrayOrNull(2);

	cout << "F : " << FScores.GetArrayOrNull(0) << endl;
	cout << "F : " << FScores.GetArrayOrNull(1) << endl;
	cout << "F : " << FScores.GetArrayOrNull(2) << endl;
	cout << "F : " << FScores.GetArrayOrNull(3) << endl;

	IScores.Add(2);
	IScores.Add(4);
	IScores.Add(22);
	IScores.Add(29);

	IScores.GetArraySize();
	cout << "I : " << IScores.GetArrayOrNull(0) << endl;
	cout << "I : " << IScores.GetArrayOrNull(1) << endl;
	cout << "I : " << IScores.GetArrayOrNull(2) << endl;
	cout << "I : " << IScores.GetArrayOrNull(3) << endl;

	return 0;
}
*/

int main()
{
	SimpleArray<int> Scores;

	Scores.Add(5);
	Scores.Add(15);
	Scores.Add(10);

	cout << Scores.GetArray(0) << endl;
	cout << Scores.GetArray(1) << endl;
	cout << Scores.GetArray(2) << endl;
	cout << Scores.GetArray(3) << endl;
	cout << Scores.GetArray(4) << endl;

	return 0;
}