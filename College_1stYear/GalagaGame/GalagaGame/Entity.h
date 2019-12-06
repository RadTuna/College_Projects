#pragma once

#include "Point.h"
#include <string>

class Entity
{
public:

	explicit Entity();
	explicit Entity(int InitPosX, int InitPosY);
	explicit Entity(const Entity& other);
	~Entity() = default;

	void SetPosition(int PosX, int PosY);
	void SetPosition(const Point& InPosition);
	void SetTag(const std::string& InTag);

	Entity& operator=(const Entity& other);

private:

	Point Position;
	std::string Tag;

public:

	__forceinline const Point& GetPosition() const { return Position; }
	__forceinline int GetPosX() const { return Position.GetPointX(); }
	__forceinline int GetPosY() const { return Position.GetPointY(); }
	__forceinline const std::string& GetTag() const { return Tag; }

};

