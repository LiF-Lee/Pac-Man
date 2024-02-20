#pragma once

#include "../Algorithm/Astar.h"
#include "../Objects/Player.h"
#include "IState.h"

class GhostBase_Hunter : public IState
{
protected:
    int frameTime;
    int startFrame;
    GameObject* gameObject;
    std::stack<Point> aStarPath {};
    virtual COLORREF GetColor() const = 0;

    static Point adjustTargetIfWall(Point target, Point player, GameMap* gameMap)
    {
        target.x = target.x < 1 ? 1 : target.x;
        target.x = target.x > gameMap->GetWidth() - 2 ? gameMap->GetWidth() - 2 : target.x;
        target.y = target.y < 1 ? 1 : target.y;
        target.y = target.y > gameMap->GetHeight() - 2 ? gameMap->GetHeight() - 2 : target.y;

        if (gameMap->GetMapData()[target.y][target.x] != Wall)
        {
            return target;
        }

        for (const auto& direction : Astar::directions)
        {
            const Point newPos = { target.x + direction.x, target.y + direction.y };
            if (gameMap->GetMapData()[newPos.y][newPos.x] != Wall)
            {
                return newPos;
            }
        }

        return player;
    }

    void PaintEstimatePath(const HDC& hdc)
    {
        constexpr int offset = DIAMETER / 2;

        const HPEN hPen = CreatePen(PS_SOLID, 2, GetColor());
        const HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));

        Point lastPoint = { gameObject->GetX(), gameObject->GetY() };

        while (!aStarPath.empty())
        {
            const Point currentPoint = aStarPath.top();

            if (!(lastPoint.y == 10
                && (lastPoint.x == 0 && currentPoint.x == gameObject->gameMap->GetWidth() - 1)
                || (lastPoint.x == gameObject->gameMap->GetWidth() - 1 && currentPoint.x == 0)))
            {
                MoveToEx(hdc, lastPoint.x * DIAMETER + offset, lastPoint.y * DIAMETER + offset, nullptr);
                LineTo(hdc, currentPoint.x * DIAMETER + offset, currentPoint.y * DIAMETER + offset);
            }
            
            lastPoint = currentPoint;
            aStarPath.pop();
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
public:
    GhostBase_Hunter(GameObject* gameObject, int startFrame = 0) : IState("Hunter"), frameTime(0), startFrame(startFrame), gameObject(gameObject) {}

    void Enter() override { }
    
    void Execute() override { }
    
    void Exit() override { }

    void Paint(const HDC& hdc) override
    {
        const int left = gameObject->GetX() * DIAMETER;
        const int top = gameObject->GetY() * DIAMETER;
        const int right = left + DIAMETER;
        const int bottom = top + DIAMETER;

        const HBRUSH MyBrush = CreateSolidBrush(GetColor());
        const auto OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
        Ellipse(hdc, left, top, right, bottom);
        
        SelectObject(hdc, OldBrush);
        DeleteObject(MyBrush);

        PaintEstimatePath(hdc);
    }
};

class GhostBlinky_Hunter : public GhostBase_Hunter
{
public:
    GhostBlinky_Hunter(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunter(gameObject, startFrame) { }

    COLORREF GetColor() const override { return RGB(255, 0, 0); }

    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;
        
        const Player* target = nullptr;
        
        for (const auto obj : *gameObject->gameObjects)
        {
            if (obj->GetTag() == "Player")
            {
                target = dynamic_cast<Player*>(obj);
                break;
            }
        }
        
        if (target == nullptr)
            return;
        
        Astar shortestPath(gameObject->gameMap);
        if (shortestPath.FindPath(gameObject->GetX(), gameObject->GetY(), target->GetX(), target->GetY()))
        {
            aStarPath = shortestPath.GetPath();
            aStarPath.pop();
        }
        
        if (startFrame > frameTime) return;
        
        if (aStarPath.empty()) return;
        
        const Point nextPoint = aStarPath.top();
        
        startFrame = nextPoint.y == 10 && (nextPoint.x <= 3 || nextPoint.x >= 15) ? frameTime + 3 : 0;
        
        gameObject->SetXY(nextPoint.x, nextPoint.y);
    }
};

class GhostInky_Hunter : public GhostBase_Hunter
{
public:
    GhostInky_Hunter(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunter(gameObject, startFrame) { }

    COLORREF GetColor() const override { return RGB(0, 252, 254); }
    
    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;
        
        const Player* target = nullptr;
        const GameObject* blinky = nullptr;
        
        for (const auto obj : *gameObject->gameObjects)
        {
            if (obj->GetTag() == "Player")
            {
                target = dynamic_cast<Player*>(obj);
            }
            if (obj->GetTag() == "Blinky")
            {
                blinky = obj;
            }
        }

        if (target == nullptr)
            return;

        Point targetPoint { target->GetX(), target->GetY() };

        switch (target->GetMovementState())
        {
        case STATE_NONE: break;
        case STATE_UP:
            targetPoint.y -= 2;
            break;
        case STATE_DOWN:
            targetPoint.y += 2;
            break;
        case STATE_LEFT:
            targetPoint.x -= 2;
            break;
        case STATE_RIGHT:
            targetPoint.x += 2;
            break;
        default: break;
        }

        targetPoint.x = targetPoint.x + (targetPoint.x - blinky->GetX());
        targetPoint.y = targetPoint.y + (targetPoint.y - blinky->GetY());
        
        targetPoint = adjustTargetIfWall(targetPoint, { target->GetX(), target->GetY() }, gameObject->gameMap);
        int targetX = targetPoint.x;
        int targetY = targetPoint.y;
        
        const double distanceToPlayer = std::sqrt(std::pow(gameObject->GetX() - target->GetX(), 2) + std::pow(gameObject->GetY() - target->GetY(), 2));
        const double distanceToTargetPoint = std::sqrt(std::pow(targetPoint.x - target->GetX(), 2) + std::pow(targetPoint.y - target->GetY(), 2));

        if (distanceToPlayer < distanceToTargetPoint)
        {
            targetX = target->GetX();
            targetY = target->GetY();
        }

        Astar shortestPath(gameObject->gameMap);
        if (shortestPath.FindPath(gameObject->GetX(), gameObject->GetY(), targetX, targetY))
        {
            aStarPath = shortestPath.GetPath();
            aStarPath.pop();
        }

        if (startFrame > frameTime) return;
        
        if (aStarPath.empty()) return;
        
        const Point nextPoint = aStarPath.top();

        startFrame = nextPoint.y == 10 && (nextPoint.x <= 3 || nextPoint.x >= 15) ? frameTime + 3 : 0;
        
        gameObject->SetXY(nextPoint.x, nextPoint.y);
    }
};

class GhostPinky_Hunter : public GhostBase_Hunter
{
public:
    GhostPinky_Hunter(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunter(gameObject, startFrame) { }

    COLORREF GetColor() const override { return RGB(252, 183, 253); }
    
    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;
        
        const Player* target = nullptr;
        
        for (const auto obj : *gameObject->gameObjects)
        {
            if (obj->GetTag() == "Player")
            {
                target = dynamic_cast<Player*>(obj);
                break;
            }
        }

        if (target == nullptr)
            return;

        Point targetPoint { target->GetX(), target->GetY() };

        switch (target->GetMovementState())
        {
        case STATE_NONE: break;
        case STATE_UP:
            targetPoint.y -= 4;
            break;
        case STATE_DOWN:
            targetPoint.y += 4;
            break;
        case STATE_LEFT:
            targetPoint.x -= 4;
            break;
        case STATE_RIGHT:
            targetPoint.x += 4;
            break;
        default: break;
        }
        
        targetPoint = adjustTargetIfWall(targetPoint, { target->GetX(), target->GetY() }, gameObject->gameMap);
        int targetX = targetPoint.x;
        int targetY = targetPoint.y;
        
        const double distanceToPlayer = std::sqrt(std::pow(gameObject->GetX() - target->GetX(), 2) + std::pow(gameObject->GetY() - target->GetY(), 2));
        const double distanceToTargetPoint = std::sqrt(std::pow(targetPoint.x - target->GetX(), 2) + std::pow(targetPoint.y - target->GetY(), 2));

        if (distanceToPlayer < distanceToTargetPoint)
        {
            targetX = target->GetX();
            targetY = target->GetY();
        }

        Astar shortestPath(gameObject->gameMap);
        if (shortestPath.FindPath(gameObject->GetX(), gameObject->GetY(), targetX, targetY))
        {
            aStarPath = shortestPath.GetPath();
            aStarPath.pop();
        }

        if (startFrame > frameTime) return;
        
        if (aStarPath.empty()) return;
        
        const Point nextPoint = aStarPath.top();

        startFrame = nextPoint.y == 10 && (nextPoint.x <= 3 || nextPoint.x >= 15) ? frameTime + 3 : 0;
        
        gameObject->SetXY(nextPoint.x, nextPoint.y);
    }
};