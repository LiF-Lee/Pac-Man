#include "GameMap.h"
#include <ostream>

void GameMap::LoadLevel_1()
{
    width = 19;
    height = 22;
    
    mapData = {
        /* 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 */
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1  }, /* 1  */
        {  1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1  }, /* 2  */
        {  1, 4, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 4, 1  }, /* 3  */
        {  1, 3, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 3, 1  }, /* 4  */
        {  1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1  }, /* 5  */
        {  1, 3, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 3, 1  }, /* 6  */
        {  1, 3, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 3, 3, 1  }, /* 7  */
        {  1, 1, 1, 1, 3, 1, 1, 1, 0, 1, 0, 1, 1, 1, 3, 1, 1, 1, 1  }, /* 8  */
        {  0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0  }, /* 9  */
        {  1, 1, 1, 1, 3, 1, 0, 1, 1, 2, 1, 1, 0, 1, 3, 1, 1, 1, 1  }, /* 10 */
        {  0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0  }, /* 11 */
        {  1, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1, 1, 0, 1, 3, 1, 1, 1, 1  }, /* 12 */
        {  0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0  }, /* 13 */
        {  1, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1, 1, 0, 1, 3, 1, 1, 1, 1  }, /* 14 */
        {  1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1  }, /* 15 */
        {  1, 3, 1, 1, 3, 1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 1, 1, 3, 1  }, /* 16 */
        {  1, 4, 3, 1, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 1, 3, 4, 1  }, /* 17 */
        {  1, 1, 3, 1, 3, 1, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 3, 1, 1  }, /* 18 */
        {  1, 3, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 3, 3, 1  }, /* 19 */
        {  1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1  }, /* 20 */
        {  1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1  }, /* 21 */
        {  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1  }, /* 22 */
    };
}

void GameMap::Paint(const HDC& hdc) const
{    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            HBRUSH MyBrush;
            HBRUSH OldBrush;

            const int left = x * DIAMETER;
            const int top = y * DIAMETER;
            const int right = left + DIAMETER;
            const int bottom = top + DIAMETER;

            const int center_x = left + (DIAMETER / 2);
            const int center_y = top + (DIAMETER / 2);
            
            switch (mapData[y][x])
            {
            case None: continue;
            case Wall:
                MyBrush = CreateSolidBrush(RGB(18, 20, 114));
                OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
                Rectangle(hdc, left, top, right, bottom);
                break;
            case TransparentWall:
                MyBrush = CreateSolidBrush(RGB(255, 211, 255));
                OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
                Rectangle(hdc, left, center_y - DIAMETER / 6, right, center_y + DIAMETER / 6);
                break;
            case Pellet:
                MyBrush = CreateSolidBrush(RGB(255, 190, 172));
                OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
                Ellipse(hdc, center_x - DIAMETER / 6,
                    center_y - DIAMETER / 6,
                    center_x + DIAMETER / 6,
                    center_y + DIAMETER / 6);
                break;
            case PowerPellet:
                MyBrush = CreateSolidBrush(RGB(255, 190, 172));
                OldBrush = static_cast<HBRUSH>(SelectObject(hdc, MyBrush));
                Ellipse(hdc, center_x - DIAMETER / 3,
                    center_y - DIAMETER / 3,
                    center_x + DIAMETER / 3,
                    center_y + DIAMETER / 3);
                break;
            default: continue;
            }
            
            SelectObject(hdc, OldBrush);
            DeleteObject(MyBrush);
        }
    }
}
