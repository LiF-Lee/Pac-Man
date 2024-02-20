#pragma once

#include "../GameObject.h"
#include "../FSM/InBox.h"

class GhostPinky : public GameObject
{
public:
    GhostPinky(int x, int y, GameMap* gameMap, std::list<GameObject*>* gameObjects)
        : GameObject("Pinky", x, y, gameMap, gameObjects)
    {
        currentState = new GhostPinky_InBox(this, 20);
        currentState->Enter();
    }

    void ChangeStateByString(std::string state, int startFrame = 0) override
    {
        if (state == "InBox")
        {
            ChangeState(new GhostPinky_InBox(this, startFrame));
        }
        else if (state == "Hunter")
        {
            ChangeState(new GhostPinky_Hunter(this, startFrame));
        }
        else if (state == "Hunted")
        {
            ChangeState(new GhostPinky_Hunted(this, startFrame));
        }
        else if (state == "Eaten")
        {
            ChangeState(new GhostPinky_Eaten(this, startFrame));
        }
    }

    void Paint(const HDC& hdc) override
    {
        currentState->Paint(hdc);
    }
};