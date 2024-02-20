#pragma once
#include <list>

#include "../GameObject.h"

enum MovementState
{
    STATE_NONE,
    STATE_UP,
    STATE_DOWN,
    STATE_LEFT,
    STATE_RIGHT
};

class Player : public GameObject
{
    int mouthAngle = 0;
    MovementState preMovementState;
    MovementState movementState;
public:
    Player(int x, int y, GameMap* gameMap, std::list<GameObject*>* gameObjects)
        : GameObject("Player", x, y, gameMap, gameObjects), preMovementState(STATE_NONE), movementState(STATE_NONE) {}
    
    MovementState GetMovementState() const { return movementState; }
    
    void ChangeMovementState(MovementState movementState) { this->preMovementState = movementState; }

    void Update() override
    {
        int newX = GetX();
        int newY = GetY();

        auto isObstacle = [&](int x, int y)
        {
            if ( x < 0 || x >= gameMap->GetWidth())
            {
                newX = x < 0 ? gameMap->GetWidth() : -1;
                movementState = x < 0 ? STATE_LEFT : STATE_RIGHT;
                return true;
            }

            if (y < 0 || y >= gameMap->GetHeight())
            {
                newY = y < 0 ? gameMap->GetHeight() : -1;
                movementState = y < 0 ? STATE_UP : STATE_DOWN;
                return true;
            }
            
            return gameMap->GetMapData()[y][x] == Wall
                || gameMap->GetMapData()[y][x] == TransparentWall;
        };

        switch (preMovementState)
        {
        case STATE_UP:
            if (!isObstacle(GetX(), GetY() - 1))
                movementState = STATE_UP;
            break;
        case STATE_DOWN:
            if (!isObstacle(GetX(), GetY() + 1))
                movementState = STATE_DOWN;
            break;
        case STATE_LEFT:
            if (!isObstacle(GetX() - 1, GetY()))
                movementState = STATE_LEFT;
            break;
        case STATE_RIGHT:
            if (!isObstacle(GetX() + 1, GetY()))
                movementState = STATE_RIGHT;
            break;
        }

        switch (movementState)
        {
        case STATE_UP: newY--; break;
        case STATE_DOWN: newY++; break;
        case STATE_LEFT: newX--; break;
        case STATE_RIGHT: newX++; break;
        default: return;
        }

        if (isObstacle(newX, newY))
        {
            movementState = STATE_NONE;
        }
        else
        {
            // mouthAngle = (mouthAngle + 1) % 3;
            SetXY(newX, newY);
        }
    }

    void Paint(const HDC& hdc) override
    {
        mouthAngle = (mouthAngle + 1) % 3;
        
        const int left = GetX() * DIAMETER;
        const int top = GetY() * DIAMETER;
        const int right = left + DIAMETER;
        const int bottom = top + DIAMETER;
        
        const int centerX = (left + right) / 2;
        const int centerY = (top + bottom) / 2;

        const int angle = 180 - mouthAngle * 20;

        const HBRUSH MyBrush = CreateSolidBrush(RGB(225, 212, 0));
        const auto OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));

        // 원 그리기
        Ellipse(hdc, left, top, right, bottom);

        if (mouthAngle == 0)
        {
            SelectObject(hdc, OldBrush);
            DeleteObject(MyBrush);
            return;
        }
        
        const HBRUSH BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, BlackBrush);
        
        switch (movementState) {
        case STATE_UP:
            Pie(hdc, left, top, right, bottom, 
                centerX + (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerY + (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerX - (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerY + (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180));
            break;
        case STATE_DOWN:
            Pie(hdc, left, top, right, bottom, 
                centerX - (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerY - (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerX + (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerY - (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180));
            break;
        case STATE_LEFT:
            Pie(hdc, left, top, right, bottom, 
                centerX + (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerY - (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerX + (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerY + (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180));
            break;
        case STATE_RIGHT:
            Pie(hdc, left, top, right, bottom, 
                centerX - (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerY + (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180),
                centerX - (DIAMETER/2) * cos(angle * 3.14159265358979323846 / 180),
                centerY - (DIAMETER/2) * sin(angle * 3.14159265358979323846 / 180));
            break;
        }

        SelectObject(hdc, OldBrush);
        DeleteObject(BlackBrush);
        DeleteObject(MyBrush);
    }

};
