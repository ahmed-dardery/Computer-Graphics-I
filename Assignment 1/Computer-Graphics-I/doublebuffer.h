#pragma once
#include <Windows.h>

HDC _hdc = nullptr, _Orghdc = nullptr;
int _win_width = 0, _win_height = 0;
HBITMAP _Membitmap = nullptr;
BOOL EndPaintDoubleBuffer(HWND hwnd, PAINTSTRUCT* ps) {
    BitBlt(_Orghdc, 0, 0, _win_width, _win_height, _hdc, 0, 0, SRCCOPY);
    DeleteObject(_Membitmap);
    DeleteDC(_Orghdc);
    return EndPaint(hwnd, ps);
}
HDC BeginPaintDoubleBuffer(HWND hwnd, PAINTSTRUCT* ps) {
    RECT Client_Rect;
    GetClientRect(hwnd, &Client_Rect);
    _win_width = Client_Rect.right - Client_Rect.left;
    _win_height = Client_Rect.bottom + Client_Rect.left;
    _Orghdc = BeginPaint(hwnd, ps);
    _hdc = CreateCompatibleDC(_Orghdc);
    _Membitmap = CreateCompatibleBitmap(_Orghdc, _win_width, _win_height);
    SelectObject(_hdc, _Membitmap);
    FillRect(_hdc, &(ps->rcPaint), CreateSolidBrush(RGB(255,255,255)));

    return _hdc;
}