#include <windows.h>
#include <algorithm>
#include <windowsx.h>
#include "doublebuffer.h"
#include "GraphicsAlgo.h"
#include "resource.h"
#include "PaintTasks.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int);
void MenuActionHandler(HWND hwnd, LPARAM wParam);
void Task1_Paint(HDC hdc, PAINTSTRUCT* ps);

int selectedTask = 1;
int main(int argc, LPWSTR argv[]) {
    return wWinMain(GetModuleHandle(NULL), NULL, argv[0], 1);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"GRAPHICS";

    WNDCLASS wc = {};   //initialize everything to null

    wc.lpfnWndProc = WndProc;     //pointer to windows procedure function
    wc.hInstance = hInstance;    //handle to instance
    wc.lpszClassName = CLASS_NAME;  //pointer to string class name
    wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName= MAKEINTRESOURCE(IDR_MENU1);
    if (!RegisterClass(&wc))
        return 0;

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Computer Graphics I",         // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    //if creation fails for any reason
    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
      
        case WM_COMMAND: {
            MenuActionHandler(hwnd, wParam);
            return 0;
        }
      
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            Tasks_Paint[selectedTask - 1](hdc, &ps);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_RBUTTONDOWN: {
            RButtonHandler(selectedTask - 1, lParam);
            
            InvalidateRect(hwnd, NULL, true);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            LButtonHandler(selectedTask - 1, lParam);
            
            InvalidateRect(hwnd, NULL, true);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MenuActionHandler(HWND hwnd, LPARAM wParam) {

    MENUITEMINFO mii = {};
    HMENU hlp = GetMenu(hwnd);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;

    const int STABLE = ID_TASK1 - 1;
    const int NTASKS = 5;
    for (int i = 1; i <= NTASKS; i++)
    {
        const int CURR = STABLE + i;
        
        mii.fState = CURR == LOWORD(wParam) ? MFS_CHECKED : MFS_UNCHECKED;
        SetMenuItemInfo(hlp, CURR, NULL, &mii);
    }
    selectedTask = LOWORD(wParam) - STABLE;
    clickCnt = circleSet = sepSet = 0;
    //GetMenuItemInfo(hlp, ID, NULL, &mii);


}