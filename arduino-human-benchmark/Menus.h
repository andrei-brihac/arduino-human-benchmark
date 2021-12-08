#ifndef MENUS_H
#define MENUS_H

#include "LcdUI.h"

enum MENU_TYPES {
  NAV, IN, GAME
};

LcdMenu *menuMain, *menuSettings, *menuUser, *menuScore, *menuAbout;

LcdText bttnMainMenu(0, 1, "Main Menu");
void initMenus() {
  menuUser = new LcdNav(
    new LcdText(0, 0, "User Settings"),
    4,
    new LcdText[4]{
      LcdText(0, 1, "Create User"),
      LcdText(0, 1, "Chose User"),
      LcdText(0, 1, "Delete User"),
      bttnMainMenu
    }
  );
  menuScore = new LcdNav(
    new LcdText(0, 0, "Score"),
    3,
    new LcdText[3]{
      LcdText(0, 1, "High Score"),
      LcdText(0, 1, "Reset"),
      bttnMainMenu
    }
  );
  menuSettings = new LcdNav(
    new LcdText(0, 0, "Settings"),
    4,
    new LcdText[4]{
      LcdText(0, 1, "LCD Contrast"),
      LcdText(0, 1, "LCD Brightness"),
      LcdText(0, 1, "LED Brightness"),
      bttnMainMenu
    }
  );
  menuAbout = new LcdNav(
    new LcdText(0, 0, "About"),
    4,
    new LcdText[4]{
      LcdText(0, 1, "HumanBenchmark"),
      LcdText(0, 1, "BrihacAndrei"),
      LcdText(0, 1, "git:todo"),
      bttnMainMenu
    }
  );
  menuMain = new LcdNav(
    new LcdText(0, 0, "Main Menu"),
    5,
    new LcdText[5]{
      LcdText(0, 1, "Start Game"),
      LcdText(0, 1, "Score", menuScore),
      LcdText(0, 1, "User Menu", menuUser),
      LcdText(0, 1, "Settings", menuSettings),
      LcdText(0, 1, "About", menuAbout)
      }
  );
  bttnMainMenu.setTargetMenu(menuMain);
  bttnMainMenu.center();
  menuUser->setBackBttn(bttnMainMenu);
  menuScore->setBackBttn(bttnMainMenu);
  menuSettings->setBackBttn(bttnMainMenu);
  menuAbout->setBackBttn(bttnMainMenu);
}

#endif
