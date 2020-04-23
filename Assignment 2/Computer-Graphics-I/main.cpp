#include <windows.h>
#include <algorithm>
#include "Win32.h"
#include "GraphicsAlgo.h"
#include "Geometry.h"
using namespace Geometry;
using namespace GraphicsAlgo;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, char* argv[]) {
    return WinMain(GetModuleHandle(NULL), NULL, argv[0], 1);
}


enum class MODE {
    NONE = 0,
    LINE,
    CIRCLE,
    CURVE,
    WINDOW
};
volatile MODE modeOfOperation = MODE::WINDOW;


DWORD WINAPI ConsoleMenu(LPVOID lpParam) {
    int op;
    while (true) {
        puts("Enter the code of the shape you would like to draw.\n1 - Line\n2 - Circle\n3 - Curve\n4 - New Window\n5 - End\n");
        scanf("%d", &op);
        if (op >= 5 || op <= 0) break;
        modeOfOperation = (MODE)op;
    }
    exit(0);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {

    CreateThread(NULL, NULL, ConsoleMenu, NULL, NULL, NULL);

    HWND hwnd = Win32::CreateFunctionalWindow(hInstance, WndProc,
        RECT{ CW_USEDEFAULT ,CW_USEDEFAULT ,800,600 },
        L"Assignment 2");

    Win32::RunMessageLoop();


    return 0;
}
const int MAXN = 100;
Circle window[MAXN]; int windowCnt = 0;
Circle circle[MAXN]; int circleCnt = 0;
Line line[MAXN]; int lineCnt = 0;
Bezier bezier[MAXN]; int bezierCnt = 0;
int bezierClickCnt = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Point cursor = { LOWORD(lParam), HIWORD(lParam) };
    switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT: {
            HDC hdc = nullptr;
            PAINTSTRUCT ps;

            hdc = Win32::BeginDoubleBufferPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 255, 255)));
            Win32::StartFastPixel(hdc);

            for(int i = 0; i < windowCnt; i++)
                DrawCircle(hdc, window[i], 0);


            for (int i = 0; i < lineCnt; i++)
                ClipLineOnCircles(hdc, window,windowCnt, line[i], RGB(0, 0, 255), RGB(255, 0, 0));

            for (int i = 0; i < circleCnt; i++)
                ClipCircleOnCircles(hdc, window,windowCnt, circle[i], RGB(0, 0, 255), RGB(255, 0, 0));

            for (int i = 0; i < bezierCnt; i++)
                ClipBezierOnCircles(hdc, window, windowCnt, bezier[i], RGB(0, 0, 255), RGB(255, 0, 0));

            for (int i = 0; i < bezierClickCnt; i++)
                DrawCircle(hdc, { bezier[bezierCnt][i],5 }, 0);

            Win32::EndFastPixel(hdc);
            Win32::EndDoubleBufferPaint(hwnd, &ps);

            return 0;
        }
        case WM_MOUSEMOVE: {
            if (wParam & MK_LBUTTON) {
                if (modeOfOperation == MODE::LINE) {
                    line[lineCnt - 1].en = cursor;
                }
                else if (modeOfOperation == MODE::CIRCLE) {
                    circle[circleCnt - 1].setRadiusFromPoint(cursor);
                }
                else if (modeOfOperation == MODE::WINDOW) {
                    window[windowCnt -1].setRadiusFromPoint(cursor);
                }
            }
            InvalidateRect(hwnd, NULL, true);
            return 0;
        }
        case WM_LBUTTONUP: {
            
            //clickCnt = (clickCnt + 1) % 2;
            return 0;
        }
        case WM_LBUTTONDOWN: {
            if (modeOfOperation == MODE::LINE) {
                line[lineCnt++] = { cursor,cursor };
            }
            else if (modeOfOperation == MODE::CIRCLE) {
                circle[circleCnt++] = { cursor,0 };
            }
            else if (modeOfOperation == MODE::WINDOW) {
                window[windowCnt++] = { cursor, 0 };
            }
            else if (modeOfOperation == MODE::CURVE) {
                bezier[bezierCnt][bezierClickCnt++] = cursor;
                if (bezierClickCnt == 4) {
                    bezierCnt++;
                    bezierClickCnt = 0;
                }
            }
            InvalidateRect(hwnd, NULL, true);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}