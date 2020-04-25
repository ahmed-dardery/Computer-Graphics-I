#include <Windows.h>
#include "MenuHandler.h"
#include "resource.h"
#include "Win32.h"
namespace MenuHandler {
    MainMenu menu;

    CheckMenu::CheckMenu() {

    }
    CheckMenu::CheckMenu(HMENU hlp, int ID) : hlp(hlp), ID(ID) {
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STATE;
        GetMenuItemInfo(hlp, ID, NULL, &mii);
    }
    bool CheckMenu::isChecked() {
        return mii.fState == MFS_CHECKED;
    }
    void CheckMenu::setChecked(bool v) {
        mii.fState = v ? MFS_CHECKED : MFS_UNCHECKED;
        SetMenuItemInfo(hlp, ID, NULL, &mii);
    }
    void CheckMenu::toggleChecked() {
        mii.fState ^= MFS_CHECKED;
        SetMenuItemInfo(hlp, ID, NULL, &mii);
    }

    int CheckMenu::getID()
    {
        return ID;
    }

    GroupMenu::GroupMenu()
    {
    }
    GroupMenu::GroupMenu(HMENU hlp, int* ids, int n)
    {
        nCnt = n;
        choices = new CheckMenu[n];
        for (int i = 0; i < n; i++)
            choices[i] = CheckMenu(hlp, ids[i]);
    }
    int GroupMenu::getCheckedIndex()
    {
        for (int i = 0; i < nCnt; i++)
            if (choices[i].isChecked()) return i;
        return -1;
    }
    void GroupMenu::setChecked(int id)
    {
        for (int i = 0; i < nCnt; i++) {
            choices[i].setChecked(choices[i].getID() == id);
        }
    }

    MainMenu::MainMenu() {

    }

    int drawingIds[] = { ID_DRAW_LINE,ID_DRAW_CIRCLE,ID_DRAW_CUBICCURVE,ID_CLIPPING_NEWWINDOW };
    int setPixelIds[] = { ID_SETPIXEL_TRADITIONAL, ID_SETPIXEL_OPTIMIZED };
    MainMenu::MainMenu(HMENU menu) :
        ModeOfOperation(menu, drawingIds, sizeof drawingIds / sizeof drawingIds[0]),
        NoClipping(menu, ID_CLIPPING_TURNOFFCLIPPING),
        SetPixel(menu, setPixelIds, sizeof setPixelIds / sizeof setPixelIds[0])
    {    }

    LPCWSTR getMenuName() {
        return MAKEINTRESOURCE(IDR_MENU1);
    }

    void initializeMenu(HWND hwnd) {
        menu = MainMenu(GetMenu(hwnd));
    }
    MainMenu Menu()
    {
        return menu;
    }
    Action performMenuAction(HWND hwnd, LPARAM wParam) {
        switch (LOWORD(wParam))
        {
            case ID_SETPIXEL_OPTIMIZED:
            case ID_SETPIXEL_TRADITIONAL: {
                menu.SetPixel.setChecked(LOWORD(wParam));
                return Action::NO_ACTION;
            }
            case ID_DRAW_LINE:
            case ID_DRAW_CIRCLE:
            case ID_DRAW_CUBICCURVE:
            case ID_CLIPPING_NEWWINDOW: {
                menu.ModeOfOperation.setChecked(LOWORD(wParam));
                return Action::NO_ACTION;
            }
            case ID_DRAW_REMOVEALLDRAWINGS: {
                return Action::REMOVE_DRAWINGS;
            }
            case ID_CLIPPING_REMOVEALL: {
                return Action::REMOVE_WINDOWS;
            }
            case ID_CLIPPING_TURNOFFCLIPPING:{
                menu.NoClipping.toggleChecked();
                return Action::NO_ACTION;
            }
            default: {
                return Action::NO_ACTION;
            }
        }


    }
}