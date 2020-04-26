#pragma once
#include <Windows.h>
#include <vector>

namespace MenuHandler {
    enum class Action {
        REMOVE_DRAWINGS,
        REMOVE_WINDOWS,
        CLEAR_SCREEN,
        NO_ACTION
    };

    enum MenuName {
        ModeOfOperation = 0,
        SetPixel = 1,
        NoClipping = 0
    };

    LPCWSTR getMenuName();
    void initializeMenu(HWND);

    class CheckMenu {
    private:
        MENUITEMINFO mii = {};
        HMENU hlp;
        int ID;
    public:
        CheckMenu(HMENU hlp, int ID);
        bool isChecked();
        void setChecked(bool v);
        void toggleChecked();
        int getID();
    };

    class GroupMenu {
    private:
        HMENU hlp;
        int startID, endID;
        int selectedIndex;
    public:
        GroupMenu(HMENU hlp, int startID, int endID, int selectedIndex);
        int getCheckedIndex();
        void setCheckedByID(int id);
        bool contains(int id);
    };

    class MainMenu {
    private:
        Action handleCheckAndGroup(int ID);
    public:
        std::vector<GroupMenu> groupMenus;
        std::vector<CheckMenu> checkMenus;
        MainMenu();
        MainMenu(HMENU menu);
        Action performMenuAction(HWND hwnd, LPARAM wParam);
    };

    MainMenu& Menu();
}