#include "Astar.h"

#include <cmath>
#include <thread>

void Astar::RelaxEdges(const Node& current, int destX, int destY)
{
    for (const auto& direction : Astar::directions)
    {
        int nx = current.x + direction.x;
        int ny = current.y + direction.y;

        nx = nx < 0 ? mapData->GetWidth() - 1 : nx >= mapData->GetWidth() ? 0 : nx;
        ny = ny < 0 ? mapData->GetHeight() - 1 : ny >= mapData->GetHeight() ? 0 : ny;

        const bool isRedZone = (current.y == 8 || current.y == 16)
            && (current.x >= 7 && current.x <= 11) ? true : false; // Red Zone
        
        if ((isRedZone && direction.y != 0)
            || mapData->GetMapData()[ny][nx] == Wall)
        {
            continue;
        }

        const int distM = ny == 10 && (nx <= 3 || nx >= 15) ? 30 : 10; // Warp Zone
        const int distG = current.cost + distM;
        const int distH = abs(destX - nx) + abs(destY - ny);
        const int newCost = distG + distH;

        if (!visitInfo[ny][nx]
            && newCost < tempMapData[ny][nx])
        {
            tempMapData[ny][nx] = newCost;
            pq.push({nx, ny, newCost});
            parent[ny][nx] = {current.x, current.y};
        }   
    }
}

bool Astar::FindPath(int sx, int sy, int dx, int dy) {
    const int width = mapData->GetWidth();
    const int height = mapData->GetHeight();

    found = false;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tempMapData[y][x] = INT_MAX;
            visitInfo[y][x] = false;
        }
    }

    pq.push({ sx, sy, 0 });
    tempMapData[sy][sx] = 0;
    parent[sy][sx] = { sx, sy };  // 시작 위치의 부모는 자기 자신

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();

        if (visitInfo[current.y][current.x])
        {
            continue; // 이미 방문한 노드는 스킵
        }

        visitInfo[current.y][current.x] = true;

        if (current.x == dx && current.y == dy)
        {
            found = true;
            break;
        }

        RelaxEdges(current, dx, dy);
    }

    // 경로 복원 부분
    if (found)
    {
        Point p = { dx, dy };
        while (p.x != sx || p.y != sy)
        {
            path.push(p);
            p = parent[p.y][p.x];
        }
        path.push({ sx, sy });  // 시작 지점 추가
        return true;
    }

    // 목적지에 도달하지 못했을 경우
    return false;
}

stack<Point> Astar::GetPath()
{
    return path;
}
