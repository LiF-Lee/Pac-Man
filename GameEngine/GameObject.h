#pragma once
#include <string>
#include <utility>

#include "Algorithm/Point.h"
#include "FSM/IState.h"

class GameObject
{
    std::string tag;
    int x, y;
protected:
    IState* currentState;
    Point previousVisitPoint {};
public:
    GameMap* gameMap;
    std::list<GameObject*>* gameObjects;
    
    GameObject() : tag("Default"), x(0), y(0), currentState(nullptr), gameMap(nullptr), gameObjects(nullptr) {}
    GameObject(std::string tag, int x, int y, GameMap* gameMap, std::list<GameObject*>* gameObjects)
        : tag(std::move(tag)), x(x), y(y), currentState(nullptr), gameMap(gameMap), gameObjects(gameObjects) {}
    virtual ~GameObject()
    {
        for (const auto obj : *gameObjects)
        {
            delete[] obj;
        }
        delete[] currentState;
        delete[] gameMap;
        delete[] gameObjects;
    }

    std::string GetTag() { return tag; }
    std::string GetStateName() const { return currentState->stateName; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    Point GetPreviousVisitPoint() const { return previousVisitPoint; }
    
    void SetTag(std::string tag) { this->tag = std::move(tag); }
    void SetXY(int x, int y) { previousVisitPoint.x = this->x; previousVisitPoint.y = this->y; this->x = x; this->y = y; }
    
    virtual bool Collision(GameObject* other)
    {
        if (this->x == other->GetX() && this->y == other->GetY())
            return true;
        return false;
    }

    virtual void Update()
    {
        currentState->Execute();
    }

    virtual void ChangeStateByString(std::string state, int startFrame = 0) { }
    
    virtual void ChangeState(IState* newState)
    {
        currentState->Exit();
        delete currentState;
        currentState = newState;
        currentState->Enter();
    }

    virtual void Paint(const HDC& hdc) = 0;
};
