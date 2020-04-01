#pragma once
class CSet
{
public:

	CSet();
	~CSet();

	// �Լ� ����.
	void Empty();
	bool Member(int e);
	bool Insert(int e);
	void Remove(int e);
	bool Equal(CSet* s);
	bool Union(CSet* inSet, CSet* outSet);
	bool Intersection(CSet* inSet, CSet* outSet);
	void Print();

	// ������ ����.
	bool operator[](int Index); // Member;
	bool operator()(int e); // Insert;
	void operator-(int e); // Remove;
	CSet operator*(CSet& other); // Intersection;
	bool operator==(CSet& other); // Equal;
	CSet operator+(CSet& other); // Union;

private:

	// MSD RadixSort
	void SortArr(int* keyArr, int keyArrLen, int maxKey);

private:

	enum { MAX = 10 };

	int Elems[MAX];
	int Num;

};

