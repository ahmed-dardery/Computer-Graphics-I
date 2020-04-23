#include <windows.h>
#include <algorithm>
#include "Win32.h"
#include "GraphicsAlgo.h"

using namespace GraphicsAlgo;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main(int argc, char* argv[]) {
    return WinMain(GetModuleHandle(NULL), NULL, argv[0], 1);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) {

    HWND hwnd = Win32::CreateFunctionalWindow(hInstance, WndProc,
        RECT{ CW_USEDEFAULT ,CW_USEDEFAULT ,800,600 },
        L"Assignment 2");

    Win32::RunMessageLoop();

    return 0;
}
Circle circle, circle2;
Line line;
int clickCnt = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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


            if (circle.radius)
                DrawCircle(hdc, circle, RGB(0, 0, 0));
            if (circle2.radius)
                ClipCircleOnCircle(hdc, circle, circle2, RGB(0, 0, 255), RGB(255, 0, 0));
            ClipLineOnCircle(hdc, circle, line, RGB(0, 0, 255), RGB(255, 0, 0));


            Win32::EndDoubleBufferPaint(hwnd, &ps);

            return 0;
        }
        case WM_MOUSEMOVE: {
            if (wParam & MK_LBUTTON) {
                if (clickCnt == 0)
                    circle.radius = hypot(LOWORD(lParam) - circle.org.x, HIWORD(lParam) - circle.org.y);
                else if (clickCnt == 1) {
                    line.en = { LOWORD(lParam), HIWORD(lParam) };
                    circle2.radius = hypot(LOWORD(lParam) - circle2.org.x, HIWORD(lParam) - circle2.org.y);
                }
                InvalidateRect(hwnd, NULL, true);
            }
            return 0;
        }
        case WM_LBUTTONUP: {
            clickCnt = (clickCnt + 1) % 2;
            return 0;
        }
        case WM_LBUTTONDOWN: {
            if (clickCnt == 0) {
                circle.org = { LOWORD(lParam), HIWORD(lParam) };
                circle.radius = 0;
            }
            else if (clickCnt == 1) {
                line.st = { LOWORD(lParam), HIWORD(lParam) };
                line.en = line.st;
                circle2.org = { LOWORD(lParam), HIWORD(lParam) };
                circle2.radius = 0;
            }
            InvalidateRect(hwnd, NULL, true);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}