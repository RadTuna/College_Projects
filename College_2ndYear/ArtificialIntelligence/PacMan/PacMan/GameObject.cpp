
#include "GameObject.h"

#include "World.h"
#include "PacMan.h"


GameObject::GameObject(World* world, EObjectLayer collisionLayer, 
    int renderChar, unsigned short renderColor, int order)
    : mWorld(world)
    , mObjectLayer(collisionLayer)
    , mRenderChar(renderChar)
    , mRenderColor(renderColor)
    , mRenderOrder(order)
    , mLocation(0, 0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{
    if (mObjectLayer == EObjectLayer::Pill)
    {
        PacMan::GetInstance()->IncreasePill();
    }
}

void GameObject::Update()
{
}

void GameObject::Destroyed()
{
    if (mObjectLayer == EObjectLayer::Pill)
    {
        PacMan::GetInstance()->DecreasePill();
    }
}

void GameObject::Render()
{
    if (mbVisible)
    {
        PacMan::GetInstance()->GetRenderer()->SetColor(mRenderColor);
        PacMan::GetInstance()->GetRenderer()->DrawChar(mLocation, mRenderChar, mRenderOrder);
    }
}

void GameObject::Destroy()
{
    GetWorld()->DestroyGameObject(this);
}

void GameObject::OnCollision(GameObject* other)
{
}

void GameObject::SetVisible(bool visible)
{
    mbVisible = visible;
}

bool GameObject::IsVisible() const
{
    return mbVisible;
}

void GameObject::SetRenderColor(unsigned short renderColor)
{
    mRenderColor = renderColor;
}

unsigned short GameObject::GetRenderColor() const
{
    return mRenderColor;
}

int GameObject::GetRenderChar() const
{
    return mRenderChar;
}

EObjectLayer GameObject::GetObjectLayer() const
{
    return mObjectLayer;
}

void GameObject::SetLocation(const Point& location)
{
    assert(mWorld != nullptr);
    mWorld->ProcessCollision(this);
    mLocation = location;
}

void GameObject::AddLocation(const Point& delta)
{
    SetLocation(Point(mLocation.X + delta.X, mLocation.Y + delta.Y));
}

const Point& GameObject::GetLocation() const
{
    return mLocation;
}

World* GameObject::GetWorld() const
{
    return mWorld;
}
