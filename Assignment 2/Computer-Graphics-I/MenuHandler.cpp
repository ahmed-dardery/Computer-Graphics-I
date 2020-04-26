#include <Windows.h>
#include "MenuHandler.h"
#include "resource.h"
#include "Win32.h"
namespace MenuHandler {
    MainMenu menu;

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
        CheckMenuItem(hlp, ID, v ? MFS_CHECKED : MFS_UNCHECKED);
    }
    void CheckMenu::toggleChecked() {
        mii.fState ^= MFS_CHECKED;
        CheckMenuItem(hlp, ID, mii.fState);
    }

    int CheckMenu::getID()
    {
        return ID;
    }

    GroupMenu::GroupMenu(HMENU hlp, int startID, int endID, int selectedIndex)
        : hlp(hlp), startID(startID), endID(endID)
    {
        setCheckedByID(selectedIndex + startID);
    }

    int GroupMenu::getCheckedIndex()
    {
        return selectedIndex;
    }
    void GroupMenu::setCheckedByID(int id) {
        CheckMenuRadioItem(hlp, startID, endID, id, MF_BYCOMMAND);
        selectedIndex = id - startID;
    }

    bool GroupMenu::contains(int id) {
        return (id >= startID && id <= endID);
    }

    MainMenu::MainMenu() {

    }

    MainMenu::MainMenu(HMENU menu)
    {
        groupMenus.push_back(GroupMenu(menu, ID_DRAW_LINE, ID_CLIPPING_NEWWINDOW, 3));
        groupMenus.push_back(GroupMenu(menu, ID_SETPIXEL_TRADITIONAL, ID_SETPIXEL_OPTIMIZED, 1));
        checkMenus.push_back(CheckMenu(menu, ID_CLIPPING_TURNOFFCLIPPING));
    }

    LPCWSTR getMenuName() {
        return MAKEINTRESOURCE(IDR_MENU1);
    }

    void initializeMenu(HWND hwnd) {
        menu = MainMenu(GetMenu(hwnd));
    }
    MainMenu& Menu()
    {
        return menu;
    }
    Action MainMenu::handleCheckAndGroup(int ID) {
        for (auto& v : groupMenus) {
            if (v.contains(ID)) {
                v.setCheckedByID(ID);
                return Action::NO_ACTION;
            }
        }
        for (auto& v : checkMenus) {
            if (v.getID() == ID) {
                v.toggleChecked();
                return Action::NO_ACTION;
            }
        }
        return Action::NO_ACTION;
    }
    Action MainMenu::performMenuAction(HWND hwnd, LPARAM wParam) {
        switch (LOWORD(wParam))
        {
            case ID_DRAW_REMOVEALLDRAWINGS: {
                return Action::REMOVE_DRAWINGS;
            }
            case ID_CLIPPING_REMOVEALL: {
                return Action::REMOVE_WINDOWS;
            }
            default: {
                return handleCheckAndGroup(LOWORD(wParam));
            }
        }


    }
}