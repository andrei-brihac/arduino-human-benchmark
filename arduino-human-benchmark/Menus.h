#ifndef MENUS_H
#define MENUS_H

#include <EEPROM.h>
#include "LcdUI.h"
#include "GameVariables.h"

enum MENU_TYPES {
  NAV, IN, GAME
};

const int eeAddrMenu = 512;

// main menu
LcdMenu* menuMain;
LcdButton bttnMainMenu(0, 1, "Main Menu");
// nav menus
const uint8_t nrOfNavMenus = 4;
LcdMenu *menuSettings, *menuUser, *menuScore, *menuAbout;
LcdMenu** navMenus[nrOfNavMenus] = {&menuSettings, &menuUser, &menuScore, &menuAbout};
// input menus
const uint8_t nrOfInputMenus = 2;
LcdMenu *menuContrast, *menuUserSet;
LcdMenu** inputMenus[nrOfInputMenus] = {&menuContrast, &menuUserSet};
// check each menu class constructor to see how to declare a menu

void initMenus() {
//  // Deleting old menu pointers.
//  int eeAddr = eeAddrMenu;
//  void* oldMenu;
//  for (uint8_t i = 0; i < 1 + nrOfNavMenus + nrOfInputMenus; i++) {
//    EEPROM.get(eeAddr, oldMenu);
//    eeAddr += 2;
//    delete oldMenu;
//  }
//  // Clear EEPROM memory where the old menu pointers were stored.
//  for (int i = 0; i < (1 + nrOfNavMenus + nrOfInputMenus) * 2; i++) {
//    EEPROM.write(i, 0);
//  }
  // Any input menu is instantiated before the nav menu containing it, making the linking easier.
  // ------------------------------------------
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
  // --------------------------------------------
  menuUserSet = new LcdInput(
    new LcdText(0, 0, "Set Username"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, userName.value),
    menuUser,
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
  // Linking input menus back to the user menu.
  ((LcdInput*)menuUserSet)->setPrevMenu(menuUser);
  // --------------------------------------------
  menuContrast = new LcdInput(
    new LcdText(0, 0, "LCD Contrast"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, String(lcdContrast.value)),
    menuSettings,
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
  // Linking input menus back to the settings menu.
  ((LcdInput*)menuContrast)->setPrevMenu(menuSettings);
  // --------------------------------------------
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
  // --------------------------------------------
  // Main menu is instantiated last because it needs to be linked to the previously instantiated menus.
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
  // Linking each back button to the main menu.
  for (uint8_t i = 0; i < nrOfNavMenus; i++) {
    ((LcdNav*)(*navMenus[i]))->setBackBttn(bttnMainMenu);
  }
//  // Save the menu pointers in EEPROM to do clean-up on next start-up.
//  eeAddr = eeAddrMenu;
//  EEPROM.put(eeAddr, (void*)menuMain);
//  eeAddr += 2;
//  for (uint8_t i = 0; i < nrOfNavMenus; i++) {
//    EEPROM.put(eeAddr, (void*)*navMenus[i]);
//    eeAddr += 2;
//  }
//  for (uint8_t i = 0; i < nrOfInputMenus; i++) {
//    EEPROM.put(eeAddr, (void*)*inputMenus[i]);
//    eeAddr += 2;
//  }
}

#endif
