#pragma once

#include "../GameObject.h"
#include "../FSM/InBox.h"

class GhostInky : public GameObject
{
public:
    GhostInky(int x, int y, GameMap* gameMap, std::list<GameObject*>* gameObjects, GameObject* blinky)
        : GameObject("Inky", x, y, gameMap, gameObjects)
    {
        currentState = new GhostInky_InBox(this, 10);
        currentState->Enter();
    }

    void ChangeStateByString(std::string state, int startFrame = 0) override
    {
        if (state == "InBox")
        {
            ChangeState(new GhostInky_InBox(this, startFrame));
        }
        else if (state == "Hunter")
        {
            ChangeState(new GhostInky_Hunter(this, startFrame));
        }
        else if (state == "Hunted")
        {
            ChangeState(new GhostInky_Hunted(this, startFrame));
        }
        else if (state == "Eaten")
        {
            ChangeState(new GhostInky_Eaten(this, startFrame));
        }
    }

    void Paint(const HDC& hdc) override
    {
        currentState->Paint(hdc);
    }
};
