#pragma once

#include <random>
#include "IState.h"

class GhostBase_Hunted : public IState
{
    Point nextPoint {};
protected:
    int frameTime;
    int startFrame;
    GameObject* gameObject;
    virtual COLORREF GetColor() const = 0;
public:
    GhostBase_Hunted(GameObject* gameObject, int startFrame = 0) : IState("Hunted"), frameTime(0), startFrame(startFrame), gameObject(gameObject) {}

    void Enter() override
    {
        nextPoint = gameObject->GetPreviousVisitPoint();
    }
    
    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;
        
        if (frameTime % 2 == 0)
        {
            gameObject->SetXY(nextPoint.x, nextPoint.y);

            vector<Point> nextPoints;
            for (const auto& direction : Astar::directions)
            {
                Point newPos = { gameObject->GetX() + direction.x, gameObject->GetY() + direction.y };

                newPos.x = newPos.x < 0 ? gameObject->gameMap->GetWidth() - 1 : newPos.x >= gameObject->gameMap->GetWidth() ? 0 : newPos.x;
                newPos.y = newPos.y < 0 ? gameObject->gameMap->GetHeight() - 1 : newPos.y >= gameObject->gameMap->GetHeight() ? 0 : newPos.y;
            
                if (!(newPos.x == gameObject->GetPreviousVisitPoint().x
                    && newPos.y == gameObject->GetPreviousVisitPoint().y)
                    && gameObject->gameMap->GetMapData()[newPos.y][newPos.x] != Wall
                    && gameObject->gameMap->GetMapData()[newPos.y][newPos.x] != TransparentWall)
                {
                    nextPoints.push_back(newPos);
                }
            }
        
            if (!nextPoints.empty())
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, nextPoints.size() - 1);
            
                nextPoint = nextPoints[dis(gen)];
            }
        }
        
        if (startFrame > frameTime) return;

        gameObject->ChangeStateByString("Hunter");
    }
    
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
    }
};

class GhostBlinky_Hunted : public GhostBase_Hunted
{
public:
    GhostBlinky_Hunted(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunted(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(29, 51, 255); }
};

class GhostInky_Hunted : public GhostBase_Hunted
{

public:
    GhostInky_Hunted(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunted(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(29, 51, 255); }
};

class GhostPinky_Hunted : public GhostBase_Hunted
{
public:
    GhostPinky_Hunted(GameObject* gameObject, int startFrame = 0) : GhostBase_Hunted(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(29, 51, 255); }
};