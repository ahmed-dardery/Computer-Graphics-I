#include "GraphicsAlgo.h"
#include <cmath>
#include <algorithm>
#define SetPixel SetPixelV
void SetMultiLine(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    DrawLine(hdc, xc, yc, xc + x, yc + y, color);
    DrawLine(hdc, xc, yc, xc - x, yc + y, color);
    DrawLine(hdc, xc, yc, xc + x, yc - y, color);
    DrawLine(hdc, xc, yc, xc - x, yc - y, color);

    DrawLine(hdc, xc, yc, xc + y, yc + x, color);
    DrawLine(hdc, xc, yc, xc - y, yc + x, color);
    DrawLine(hdc, xc, yc, xc + y, yc - x, color);
    DrawLine(hdc, xc, yc, xc - y, yc - x, color);
}

void SetMultiPixel(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);

    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);

    //DrawLine(hdc, xc, yc, xc + y, yc + x, color);
    //DrawLine(hdc, xc, yc, xc - y, yc - x, color);
}
void DrawQuadrants(HDC hdc, int xc, int yc, int R, COLORREF color) {
    DrawLine(hdc, xc, yc, xc + R, yc, color);
    DrawLine(hdc, xc, yc, xc - R, yc, color);
    DrawLine(hdc, xc, yc, xc, yc + R, color);
    DrawLine(hdc, xc, yc, xc, yc - R, color);

    int h = R * sqrt(2) / 2;
    DrawLine(hdc, xc, yc, xc + h, yc + h, color);
    DrawLine(hdc, xc, yc, xc - h, yc + h, color);
    DrawLine(hdc, xc, yc, xc + h, yc - h, color);
    DrawLine(hdc, xc, yc, xc - h, yc - h, color);
}
void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color, MultiPixelDrawer func) {
    int x = R;
    int y = 0;

    
    func(hdc,xc,yc,color,x, y);
    func(hdc, xc, yc, color, x, y+1);

    int d = 5 - 4 * R;
    while (x > y)
    {
        if (d > 0) {
            d += -8 * x + 8;
            x--;
        }
        d += 8 * y + 12;
        y++;

        func(hdc, xc, yc, color, x,y);
        func(hdc, xc, yc, color, x, y + 1);

    }
}

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    if (abs(dx) >= abs(dy)) {
        if (xe < xs) {
            std::swap(xs, xe), std::swap(ys, ye);
            dx *= -1;
            dy *= -1;
        }
        SetPixel(hdc, xs, ys, color);

        int x = xs;
        int y = ys;
        int diff = ye > ys ? 1 : -1;
        int d = 2 * dy - diff * dx;
        
        while (x < xe) {
            ++x;
            if (d * diff > 0) {
                y += diff, d -= 2 * dx * diff;
            }
            d += 2 * dy;
            SetPixel(hdc, x, y-1, color);
            SetPixel(hdc, x, y, color);
        }
    }
    else {
        if (ye < ys) {
            std::swap(xs, xe), std::swap(ys, ye);
            dx *= -1;
            dy *= -1;
        }
        SetPixel(hdc, xs, ys, color);

        int x = xs;
        int y = ys;
        int diff = xe > xs ? 1 : -1;
        int d = dy * diff - 2 * dx;

        while (y < ye) {
            if (d * diff < 0) {
                x += diff, d += 2 * dy * diff;
            }
            ++y;
            d -= 2 * dx;
            SetPixel(hdc, x-1, y, color);
            SetPixel(hdc, x, y, color);
        }
    }
}
