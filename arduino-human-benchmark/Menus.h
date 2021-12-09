#ifndef MENUS_H
#define MENUS_H

#include "LcdUI.h"
#include "GameVariables.h"

enum MENU_TYPES {
  NAV, IN, GAME
};

LcdMenu *menuMain, *menuSettings, *menuUser, *menuScore, *menuAbout;  // nav menus
LcdMenu *menuContrast, *menuUserSet;  // input menus
// check each menu class constructor to see how to declare a menu

LcdButton bttnMainMenu(0, 1, "Main Menu");
void initMenus() {
  menuUserSet = new LcdInput(
    new LcdText(0, 0, "Set Username"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, userName),
    menuMain,
    &setUserName
  );
  menuUser = new LcdNav(
    new LcdText(0, 0, "User Settings"),
    MENU_TYPES::NAV,
    3,
    new LcdButton[3]{
      LcdButton(0, 1, "Set Username", menuUserSet),
      LcdButton(0, 1, "Delete User"),
      bttnMainMenu
    }
  );
  menuScore = new LcdNav(
    new LcdText(0, 0, "Score"),
    MENU_TYPES::NAV,
    3,
    new LcdButton[3]{
      LcdButton(0, 1, "High Score"),
      LcdButton(0, 1, "Reset"),
      bttnMainMenu
    }
  );
  menuContrast = new LcdInput(
    new LcdText(0, 0, "LCD Contrast"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, lcdContrast),
    menuMain,
    &setLcdContrast
  );
  menuSettings = new LcdNav(
    new LcdText(0, 0, "Settings"),
    MENU_TYPES::NAV,
    4,
    new LcdButton[4]{
      LcdButton(0, 1, "LCD Contrast", menuContrast),
      LcdButton(0, 1, "LCD Brightness"),
      LcdButton(0, 1, "LED Brightness"),
      bttnMainMenu
    }
  );
  menuAbout = new LcdNav(
    new LcdText(0, 0, "About"),
    MENU_TYPES::NAV,
    4,
    new LcdButton[4]{
      LcdButton(0, 1, "HumanBenchmark"),
      LcdButton(0, 1, "BrihacAndrei"),
      LcdButton(0, 1, "git:todo"),
      bttnMainMenu
    }
  );
  menuMain = new LcdNav(
    new LcdText(0, 0, "Main Menu"),
    MENU_TYPES::NAV,
    5,
    new LcdButton[5]{
      LcdButton(0, 1, "Start Game"),
      LcdButton(0, 1, "Score", menuScore),
      LcdButton(0, 1, "User Menu", menuUser),
      LcdButton(0, 1, "Settings", menuSettings),
      LcdButton(0, 1, "About", menuAbout)
      }
  );
  bttnMainMenu.setTargetMenu(menuMain);
  bttnMainMenu.center();
  ((LcdNav*)menuUser)->setBackBttn(bttnMainMenu);
  ((LcdNav*)menuScore)->setBackBttn(bttnMainMenu);
  ((LcdNav*)menuSettings)->setBackBttn(bttnMainMenu);
  ((LcdNav*)menuAbout)->setBackBttn(bttnMainMenu);

  ((LcdInput*)menuContrast)->setPrevMenu(menuMain);
  ((LcdInput*)menuUserSet)->setPrevMenu(menuMain);
}

#endif
