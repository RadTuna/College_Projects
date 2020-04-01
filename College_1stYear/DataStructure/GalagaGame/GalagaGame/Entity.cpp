#include "Entity.h"

Entity::Entity()
	: Position(0, 0)
	, Tag("Entity")
{
}

Entity::Entity(int InitPosX, int InitPosY)
	: Position(InitPosX, InitPosY)
	, Tag("Entity")
{
}

Entity::Entity(const Entity& other)
{
	Position = other.Position;
	Tag = other.Tag;
}

void Entity::SetPosition(int PosX, int PosY)
{
	Position.SetPoint(PosX, PosY);
}

void Entity::SetPosition(const Point& InPosition)
{
	Position = InPosition;
}

void Entity::SetTag(const std::string& InTag)
{
	Tag = InTag;
}

Entity& Entity::operator=(const Entity& other)
{
	Position = other.Position;
	Tag = other.Tag;
	return *this;
}
