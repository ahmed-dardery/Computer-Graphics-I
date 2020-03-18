#pragma once
#include <Windows.h>
#include "GraphicsAlgo.h"
int Tasks_LClickCount[] = { 2,1,1,3,3 };

void FillTwoQuadrants(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);

    DrawLine(hdc, xc, yc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    DrawLine(hdc, xc, yc, xc - y, yc - x, color);

}
void ColorfulQuadrants(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    static COLORREF colors[] = {
        RGB(255, 0, 0),
        RGB(128,0,0),
        RGB(0,255,0),
        RGB(0,128,0),
        RGB(0,0,255),
        RGB(0,0,128),
        RGB(255,255,0),
        RGB(128,128,0)
    };
    DrawLine(hdc, xc, yc, xc + x, yc + y, colors[0]);
    DrawLine(hdc, xc, yc, xc - x, yc - y, colors[1]);
    DrawLine(hdc, xc, yc, xc - x, yc + y, colors[2]);
    DrawLine(hdc, xc, yc, xc + x, yc - y, colors[3]);

    DrawLine(hdc, xc, yc, xc + y, yc + x, colors[4]);
    DrawLine(hdc, xc, yc, xc - y, yc - x, colors[5]);
    DrawLine(hdc, xc, yc, xc - y, yc + x, colors[6]);
    DrawLine(hdc, xc, yc, xc + y, yc - x, colors[7]);
}

double MULTIPLYBY; bool dropFirstQuadrant;
void RingDrawer(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
    int xx =  x* MULTIPLYBY;
    int yy = y * MULTIPLYBY;
    static COLORREF colors[] = {
        RGB(255, 0, 0),
        RGB(128,0,0),
        RGB(0,255,0),
        RGB(0,128,0),
        RGB(0,0,255),
        RGB(0,0,128),
        RGB(255,255,0),
        RGB(128,128,0)
    };
    DrawLine(hdc, xc + xx, yc + yy, xc + x, yc + y, colors[0]);
    DrawLine(hdc, xc - xx, yc - yy, xc - x, yc - y, colors[1]);
    DrawLine(hdc, xc - xx, yc + yy, xc - x, yc + y, colors[2]);
    DrawLine(hdc, xc + xx, yc - yy, xc + x, yc - y, colors[3]);

    DrawLine(hdc, xc + yy, yc + xx, xc + y, yc + x, colors[4]);
    DrawLine(hdc, xc - yy, yc - xx, xc - y, yc - x, colors[5]);
    DrawLine(hdc, xc - yy, yc + xx, xc - y, yc + x, colors[6]);
    DrawLine(hdc, xc + yy, yc - xx, xc + y, yc - x, colors[7]);
}

int lc_x[3], lc_y[3], clickCnt = 0;
int rc_x, rc_y;
bool circleSet, sepSet;

void RButtonHandler(int idx, LPARAM lParam) {
    rc_x = LOWORD(lParam);
    rc_y = HIWORD(lParam);
    sepSet = 1;
}

void LButtonHandler(int idx, LPARAM lParam) {
    int totalCount = Tasks_LClickCount[idx];
    sepSet = 0;
    lc_x[clickCnt] = LOWORD(lParam);
    lc_y[clickCnt] = HIWORD(lParam);
    if (++clickCnt >= totalCount) {
        clickCnt = 0;
        circleSet = 1;
    }
    else {
        circleSet = 0;
    }
}

void Task1_Paint(HDC hdc, PAINTSTRUCT* ps) {
    if (circleSet && sepSet) {
        int dx1 = lc_x[1] - lc_x[0];
        int dy1 = lc_y[1] - lc_y[0];
        int dx2 = rc_x - lc_x[0];
        int dy2 = rc_y - lc_y[0];

        int dot = dx1 * dx2 + dy1 * dy2;

        int dist = dx1 * dx1 + dy1 * dy1;
        if (dist == 0) return;

        int xx = dot * dx1 / dist + lc_x[0];
        int yy = dot * dy1 / dist + lc_y[0];

        int r1 = hypot(xx - lc_x[0], yy - lc_y[0]);
        int r2 = hypot(xx - lc_x[1], yy - lc_y[1]);
        DrawCircle(hdc, lc_x[0], lc_y[0], r1, 0, FillTwoQuadrants);
        DrawCircle(hdc, lc_x[1], lc_y[1], r2, 0);
    }
    else {


        if (circleSet) {
            DrawCircle(hdc, lc_x[0], lc_y[0], 5, 0);
            DrawCircle(hdc, lc_x[1], lc_y[1], 5, 0);
        }
        else if (clickCnt == 1) {
            DrawCircle(hdc, lc_x[0], lc_y[0], 5, 0);
        }

    }
}

void Task2_Paint(HDC hdc, PAINTSTRUCT* ps) {
    if (circleSet && sepSet) {
        int R = hypot(rc_x - lc_x[0], rc_y - lc_y[0]);
        DrawCircle(hdc, lc_x[0], lc_y[0], R, 0);
        DrawQuadrants(hdc, lc_x[0], lc_y[0], R, 0);
    }
    else if (circleSet) {
        DrawCircle(hdc, lc_x[0], lc_y[0], 5, 0);
    }
}

void Task3_Paint(HDC hdc, PAINTSTRUCT* ps) {
    if (circleSet && sepSet) {
        int R = hypot(rc_x - lc_x[0], rc_y - lc_y[0]);
        DrawCircle(hdc, lc_x[0], lc_y[0], R, 0, ColorfulQuadrants);
    }
    else if (circleSet) {
        DrawCircle(hdc, lc_x[0], lc_y[0], 5, 0);
    }
}
void Task4_Paint(HDC hdc, PAINTSTRUCT* ps) {
    if (circleSet) {
        int R1 = hypot(lc_x[1] - lc_x[0], lc_y[1] - lc_y[0]);
        int R2 = hypot(lc_x[2] - lc_x[0], lc_y[2] - lc_y[0]);
        MULTIPLYBY = 1.0*R1 / R2;
        dropFirstQuadrant = 0;
        DrawCircle(hdc, lc_x[0], lc_y[0], R2, 0, RingDrawer);
        DrawLine(hdc, lc_x[0], lc_y[0], lc_x[1], lc_y[1], 0);
        DrawLine(hdc, lc_x[0], lc_y[0], lc_x[2], lc_y[2], 0);
    }
    else {
        for (int i = 0; i < clickCnt; i++)
            DrawCircle(hdc, lc_x[i], lc_y[i], 5, 0);
    }
}
void Task5_Paint(HDC hdc, PAINTSTRUCT* ps) {
    if (circleSet) {
        int R1 = hypot(lc_x[1] - lc_x[0], lc_y[1] - lc_y[0]);
        int R2 = hypot(lc_x[2] - lc_x[0], lc_y[2] - lc_y[0]);
        MULTIPLYBY = 1.0 * R1 / R2;
        dropFirstQuadrant = 1;
        DrawCircle(hdc, lc_x[0], lc_y[0], R2, 0, RingDrawer);
    }
    else {
        for (int i = 0; i < clickCnt; i++)
            DrawCircle(hdc, lc_x[i], lc_y[i], 5, 0);
    }
}
void (*Tasks_Paint[])(HDC, PAINTSTRUCT*) = { Task1_Paint, Task2_Paint, Task3_Paint, Task4_Paint, Task5_Paint };
