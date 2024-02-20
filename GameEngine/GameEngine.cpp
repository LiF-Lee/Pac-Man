#include "GameEngine.h"

#include "Objects/Player.h"

void GameEngine::Update()
{
    frameTime = (frameTime + 1) % 2147483647;
    if (gameEnd || startFrame > frameTime) return;
    
    for (const auto gameObject : *gameObjects)
    {
        gameObject->Update();
    }

    CheckCollisions();
}

void GameEngine::Paint(const HDC& hdc) const
{
    gameMap->Paint(hdc);
    for (const auto gameObject : *gameObjects)
    {
        gameObject->Paint(hdc);
    }
}

void GameEngine::AddGameObject(GameObject* gameObject) const
{
    gameObjects->push_back(gameObject);
}

void GameEngine::HitEnemy(GameObject* gameObject)
{
    if (gameObject->GetStateName() == "Hunter")
    {
        gameEnd = true;
    }
    else if (gameObject->GetStateName() == "Hunted")
    {
        gameObject->ChangeStateByString("Eaten", 15);
    }
}

void GameEngine::HitPellet(int x, int y) const
{
    gameMap->GetMapData()[y][x] = 0;
}

void GameEngine::HitPowerPellet(int x, int y) const
{
    gameMap->GetMapData()[y][x] = 0;
    for (const auto obj : *gameObjects)
    {
        if (obj->GetTag() != "Player"
            && (obj->GetStateName() == "Hunter" || obj->GetStateName() == "Hunted"))
        {
            obj->ChangeStateByString("Hunted", 30);
        }
    }
}

void GameEngine::CheckCollisions()
{
    if (player == nullptr)
        return;
    
    for (const auto gameObject : *gameObjects)
    {
        if (gameObject->GetTag() == "Player"
            || !player->Collision(gameObject))
            continue;
        HitEnemy(gameObject);
    }

    switch (gameMap->GetMapData()[player->GetY()][player->GetX()])
    {
    case Pellet:
        HitPellet(player->GetX(), player->GetY());
        break;
    case PowerPellet:
        HitPowerPellet(player->GetX(), player->GetY());
        break;
    default: break;
    }
}
