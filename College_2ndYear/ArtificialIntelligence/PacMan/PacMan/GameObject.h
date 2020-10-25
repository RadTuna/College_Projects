#pragma once

#include "Point.h"
#include "TypeDefinition.h"

class GameObject
{
public:
    explicit GameObject(class World* world, EObjectLayer collisionLayer, 
        int renderChar, unsigned short renderColor, int order);
    virtual ~GameObject();

    virtual void Awake();
    virtual void Update();
    virtual void Destroyed();
    void Render();
    void Destroy();

    virtual void OnCollision(GameObject* other);

    void SetVisible(bool visible);
    bool IsVisible() const;

    void SetRenderColor(unsigned short renderColor);
    unsigned short GetRenderColor() const;

    int GetRenderChar() const;
    EObjectLayer GetObjectLayer() const;

    void SetLocation(const Point& location);
    void AddLocation(const Point& delta);
    const Point& GetLocation() const;
    class World* GetWorld() const;

private:
    Point mLocation;

    int mRenderChar;
    unsigned short mRenderColor;
    int mRenderOrder;
    EObjectLayer mObjectLayer;

    bool mbVisible = true;
    class World* mWorld = nullptr;

};
