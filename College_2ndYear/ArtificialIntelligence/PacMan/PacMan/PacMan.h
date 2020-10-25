#pragma once

#include <vector>

#include "World.h"
#include "ConsoleRenderer.h"

class PacMan final
{
public:
    ~PacMan();

    static void CreateInstance(int tickInterval);
    static PacMan* GetInstance();
    static void DeleteInstance();

    void Run();
    void LoadMap(const char* filePath);

    ConsoleRenderer* GetRenderer() const;
    void GameEnd();

    void IncreasePill();
    void DecreasePill();
    size_t GetPillCount() const;

private:
    explicit PacMan(int tickInterval);

private:
    void Awake();
    void Update();

private:
    static PacMan* mSelf;

    size_t mPillCount;
    int mTickInterval;
    bool mbIsGameEnd = false;

    std::unique_ptr<World> mWorld;
    std::unique_ptr<ConsoleRenderer> mConsoleRenderer;

};
