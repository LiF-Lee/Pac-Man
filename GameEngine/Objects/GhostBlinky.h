#pragma once

#include "../GameObject.h"
#include "../FSM/InBox.h"
#include "../FSM/Hunter.h"
#include "../FSM/Hunted.h"
#include "../FSM/Eaten.h"

class GhostBlinky : public GameObject
{
public:
    GhostBlinky(int x, int y, GameMap* gameMap, std::list<GameObject*>* gameObjects)
        : GameObject("Blinky", x, y, gameMap, gameObjects)
    {
        currentState = new GhostBlinky_InBox(this, 0);
        currentState->Enter();
    }

    void ChangeStateByString(std::string state, int startFrame = 0) override
    {
        if (state == "InBox")
        {
            ChangeState(new GhostBlinky_InBox(this, startFrame));
        }
        else if (state == "Hunter")
        {
            ChangeState(new GhostBlinky_Hunter(this, startFrame));
        }
        else if (state == "Hunted")
        {
            ChangeState(new GhostBlinky_Hunted(this, startFrame));
        }
        else if (state == "Eaten")
        {
            ChangeState(new GhostBlinky_Eaten(this, startFrame));
        }
    }

    void Paint(const HDC& hdc) override
    {
        currentState->Paint(hdc);
    }
};
