
#include "PacMan.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "Ghost.h"
#include "Player.h"
#include "TypeDefinition.h"


PacMan* PacMan::mSelf = nullptr;

PacMan::PacMan(int tickInterval)
    : mTickInterval(tickInterval)
{
    mConsoleRenderer = std::make_unique<ConsoleRenderer>(30, 30);
}

PacMan::~PacMan()
{
}

void PacMan::CreateInstance(int tickInterval)
{
    if (mSelf == nullptr)
    {
        mSelf = new PacMan(tickInterval);
    }
}

PacMan* PacMan::GetInstance()
{
    return mSelf;
}

void PacMan::DeleteInstance()
{
    if (mSelf != nullptr)
    {
        delete mSelf;
        mSelf = nullptr;
    }
}

void PacMan::Run()
{
    Awake();

    while (!mbIsGameEnd)
    {
        Update();
    }
}

void PacMan::Awake()
{
}

void PacMan::Update()
{
    mWorld->Update();
    mConsoleRenderer->Present();

    Sleep(mTickInterval);
}

void PacMan::LoadMap(const char* filePath)
{
    assert(std::filesystem::exists(filePath));

    std::ifstream fileStream(filePath);
    assert(fileStream.is_open());

    std::string headerLine;
    std::getline(fileStream, headerLine);
    const size_t row = std::stoull(headerLine);

    std::getline(fileStream, headerLine);
    const size_t column = std::stoull(headerLine);

    mWorld = std::make_unique<World>(row, column);


    char currentChar = fileStream.get();
    Point currentLocation;
    while (currentChar != -1 && !fileStream.eof())
    {
        if (currentChar == '\n')
        {
            currentChar = fileStream.get();
            currentLocation.X = 0;
            currentLocation.Y = currentLocation.Y + 1;
            continue;
        }

        switch (currentChar)
        {
        case '%': // Wall
            mWorld->SpawnGameObject<GameObject>(currentLocation, EObjectLayer::Wall, L'#', HIGH | WHITE, 0);
            mWorld->SetNavMap(currentLocation, ENavType::Obstacle);
            break;
        case '*': // Ghost
            mWorld->SpawnGameObject<Ghost>(currentLocation, EObjectLayer::Ghost, L'G', HIGH | RED, 1);
            break;
        case '@': // Power Pill
            mWorld->SpawnGameObject<GameObject>(currentLocation, EObjectLayer::PowerPill, L'*', HIGH | WHITE, 0);
            break;
        case ' ': // Pill
            mWorld->SpawnGameObject<GameObject>(currentLocation, EObjectLayer::Pill, L'.', HIGH | WHITE, 0);
            break;
        case '&': // Player
            mWorld->SpawnGameObject<Player>(currentLocation, EObjectLayer::Player, L'P', HIGH | GREEN, 2);
            break;
        case '#': // Empty
            break;
        default:
            assert(false);
            break;
        }

        currentChar = fileStream.get();
        currentLocation.X = currentLocation.X + 1;
    }

    fileStream.close();
}

ConsoleRenderer* PacMan::GetRenderer() const
{
    return mConsoleRenderer.get();
}

void PacMan::GameEnd()
{
    mbIsGameEnd = true;
}

void PacMan::IncreasePill()
{
    ++mPillCount;
}

void PacMan::DecreasePill()
{
    --mPillCount;
    if (mPillCount == 0)
    {
        GameEnd();
    }
}

size_t PacMan::GetPillCount() const
{
    return mPillCount;
}
