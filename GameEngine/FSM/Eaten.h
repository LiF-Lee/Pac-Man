#pragma once

#include "IState.h"

class GhostBase_Eaten : public IState
{
protected:
    int frameTime;
    int startFrame;
    GameObject* gameObject;
    virtual COLORREF GetColor() const = 0;
public:
    GhostBase_Eaten(GameObject* gameObject, int startFrame = 0) : IState("Eaten"), frameTime(0), startFrame(startFrame), gameObject(gameObject) {}

    void Enter() override { }
    
    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;

        if (startFrame > frameTime) return;

        gameObject->ChangeStateByString("InBox");
    }
    
    void Exit() override { }

    void Paint(const HDC& hdc) override
    {
        const int left = gameObject->GetX() * DIAMETER;
        const int top = gameObject->GetY() * DIAMETER;
        const int right = left + DIAMETER;
        const int bottom = top + DIAMETER;

        const int center_x = left + (DIAMETER / 2);
        const int center_y = top + (DIAMETER / 2);

        const HBRUSH MyBrush = CreateSolidBrush(GetColor());
        const auto OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
        Ellipse(hdc, center_x - DIAMETER / 4,
                    center_y - DIAMETER / 4,
                    center_x + DIAMETER / 4,
                    center_y + DIAMETER / 4);
        SelectObject(hdc, OldBrush);
        DeleteObject(MyBrush);
    }
};

class GhostBlinky_Eaten : public GhostBase_Eaten
{
public:
    GhostBlinky_Eaten(GameObject* gameObject, int startFrame = 0) : GhostBase_Eaten(gameObject, startFrame) {}

    void Enter() override
    {
        gameObject->SetXY(10, 10);
    }
    
    COLORREF GetColor() const override { return RGB(255, 0, 0); }
};

class GhostInky_Eaten : public GhostBase_Eaten
{
public:
    GhostInky_Eaten(GameObject* gameObject, int startFrame = 0) : GhostBase_Eaten(gameObject, startFrame) {}

    void Enter() override
    {
        gameObject->SetXY(8, 10);
    }
    
    COLORREF GetColor() const override { return RGB(0, 252, 254); }
};

class GhostPinky_Eaten : public GhostBase_Eaten
{
public:
    GhostPinky_Eaten(GameObject* gameObject, int startFrame = 0) : GhostBase_Eaten(gameObject, startFrame) {}

    void Enter() override
    {
        gameObject->SetXY(9, 10);
    }
    
    COLORREF GetColor() const override { return RGB(252, 183, 253); }
};