#pragma once

#include <string>
#include <memory>

using namespace std;

class Node final
{
public:
	Node() = delete;
	Node(const char* Key, int Date);
	~Node() = default;

	void SetNext(const shared_ptr<Node> next);
	shared_ptr<Node> GetNext() const;
	int GetDate() const;
	const string& GetKey() const;

private:
	string mKey;
	int mData;
	shared_ptr<Node> mNext;
};