#pragma once

#include "IState.h"

class GhostBase_InBox : public IState
{
protected:
    int frameTime;
    int startFrame;
    GameObject* gameObject;
    virtual COLORREF GetColor() const = 0;
public:
    GhostBase_InBox(GameObject* gameObject, int startFrame = 0) : IState("InBox"), frameTime(0), startFrame(startFrame), gameObject(gameObject) {}

    void Enter() override { }
    
    void Execute() override
    {
        frameTime = (frameTime + 1) % 2147483647;

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

class GhostBlinky_InBox : public GhostBase_InBox
{
public:
    GhostBlinky_InBox(GameObject* gameObject, int startFrame = 0) : GhostBase_InBox(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(255, 0, 0); }
};

class GhostInky_InBox : public GhostBase_InBox
{

public:
    GhostInky_InBox(GameObject* gameObject, int startFrame = 0) : GhostBase_InBox(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(0, 252, 254); }
};

class GhostPinky_InBox : public GhostBase_InBox
{
public:
    GhostPinky_InBox(GameObject* gameObject, int startFrame = 0) : GhostBase_InBox(gameObject, startFrame) {}
    
    COLORREF GetColor() const override { return RGB(252, 183, 253); }
};