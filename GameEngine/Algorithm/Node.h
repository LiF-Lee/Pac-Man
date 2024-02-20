#pragma once

struct Node {
    int x, y, cost;
    bool operator<(const Node& other) const
    {
        return cost > other.cost;
    }
};