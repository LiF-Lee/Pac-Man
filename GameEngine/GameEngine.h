#pragma once
#include <list>
#include <Windows.h>
#include "GameMap.h"
#include "GameObject.h"
#include "Objects/Player.h"
#include "Objects/GhostBlinky.h"
#include "Objects/GhostPinky.h"
#include "Objects/GhostInky.h"

class GameEngine
{
    int frameTime;
    int startFrame;
    bool gameEnd;

    Player* player;
    GameMap* gameMap;
    std::list<GameObject*>* gameObjects;
public:
    GameEngine() : frameTime(0), startFrame(15), gameEnd(false)
    {
        gameMap = new GameMap;
        gameMap->LoadLevel_1();
        SetGameMap(gameMap);

        gameObjects = new std::list<GameObject*>();

        player = new Player(9, 16, gameMap, gameObjects);
        const auto Blinky = new GhostBlinky(9, 8, gameMap, gameObjects);
        const auto Pinky = new GhostPinky(9, 10, gameMap, gameObjects);
        const auto Inky = new GhostInky(8, 10, gameMap, gameObjects, Blinky);
        
        AddGameObject(player);
        AddGameObject(Blinky);
        AddGameObject(Pinky);
        AddGameObject(Inky);
    }

    void Update();
    void Paint(const HDC& hdc) const;

    void ChangePlayerMovementState(MovementState movementState) const { player->ChangeMovementState(movementState); }

    void SetGameMap(GameMap* map) { gameMap = map; }

    GameMap* GetGameMap() const { return gameMap; }
    Player* GetPlayer() const { return player; }
    std::list<GameObject*>* GetGameObjects() const { return gameObjects; }
    
    void AddGameObject(GameObject* gameObject) const;

    void HitEnemy(GameObject* gameObject);
    void HitPellet(int x, int y) const;
    void HitPowerPellet(int x, int y) const;
    
    void CheckCollisions();
};
