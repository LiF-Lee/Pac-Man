#pragma once

class IState
{
public:
    std::string stateName;
    IState(std::string stateName = "Default") : stateName(stateName) {}
    virtual ~IState() {}
    virtual void Enter() = 0;
    virtual void Execute() = 0;
    virtual void Exit() = 0;
    virtual void Paint(const HDC& hdc) = 0;
};
