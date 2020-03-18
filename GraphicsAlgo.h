#pragma once
#include <Windows.h>
#include <math.h>
#define ROUND(v) int(v + (v>0?0.5:-0.5))

typedef void (*MultiPixelDrawer)(HDC, int, int, COLORREF, int, int);

void SetMultiPixel(HDC hdc, int xc, int yc, COLORREF color, int x, int y);
void SetMultiLine(HDC hdc, int xc, int yc, COLORREF color, int x, int y);

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);
void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color, MultiPixelDrawer func = SetMultiPixel);
void DrawQuadrants(HDC hdc, int xc, int yc, int R, COLORREF color);

void FillRing(HDC hdc, int xc, int yc, int R1, int R2, COLORREF color);