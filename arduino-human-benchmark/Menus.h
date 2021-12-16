#ifndef MENUS_H
#define MENUS_H

#include <EEPROM.h>
#include "LcdUI.h"
#include "GameVariables.h"

enum MENU_TYPES {
  NAV, IN, GAME
};

const int addrMenu = 512;

// main menu
LcdMenu* menuMain;
LcdButton bttnMainMenu(0, 1, "Main Menu");
// nav menus
const uint8_t nrOfNavMenus = 4;
LcdMenu *menuSettings, *menuUser, *menuScore, *menuAbout;
LcdMenu** navMenus[nrOfNavMenus] = {&menuSettings, &menuUser, &menuScore, &menuAbout};
// input menus
const uint8_t nrOfInputMenus = 4;
LcdMenu *menuContrastLcd, *menuUserSet, *menuBrightnessLcd, *menuBrightnessLed;
LcdMenu** inputMenus[nrOfInputMenus] = {&menuContrastLcd, &menuUserSet, &menuBrightnessLcd, &menuBrightnessLed};
// combat menu
LcdMenu* menuGame;
// check each menu class constructor to see how to declare a menu

void initMenus() {
//  // Deleting old menu pointers.
//  int addr = addrMenu;
//  void* oldMenu;
//  for (uint8_t i = 0; i < 1 + nrOfNavMenus + nrOfInputMenus; i++) {
//    EEPROM.get(addr, oldMenu);
//    addr += 2;
//    delete oldMenu;
//  }
  // Any input menu is instantiated before the nav menu containing it, making the linking easier.
  menuGame = new LcdGame(new LcdText(0, 0, "Game Menu"), MENU_TYPES::GAME);
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
  menuContrastLcd = new LcdInput(
    new LcdText(0, 0, "LCD Contrast"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, String(lcdContrast.value)),
    menuSettings,
    &setLcdContrast
  );
  menuBrightnessLcd = new LcdInput(
    new LcdText(0, 0, "LCD Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, lcdBrightness.value),
    menuSettings,
    &setLcdBrightness
  );
  menuBrightnessLed = new LcdInput(
    new LcdText(0, 0, "LED Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(0, 1, ledBrightness.value),
    menuSettings,
    &setLedBrightness
  );
  menuSettings = new LcdNav(
    new LcdText(0, 0, "Settings"),
    MENU_TYPES::NAV,
    4,
    new LcdButton[4]{
      LcdButton(0, 1, "LCD Contrast", menuContrastLcd),
      LcdButton(0, 1, "LCD Brightness", menuBrightnessLcd),
      LcdButton(0, 1, "LED Brightness", menuBrightnessLed),
      bttnMainMenu
    }
  );
  // Linking input menus back to the settings menu.
  ((LcdInput*)menuContrastLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLed)->setPrevMenu(menuSettings);
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
      LcdButton(0, 1, "Start Game", menuGame),
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
//  addr = addrMenu;
//  EEPROM.put(addr, (void*)menuMain);
//  addr += 2;
//  for (uint8_t i = 0; i < nrOfNavMenus; i++) {
//    EEPROM.put(addr, (void*)*navMenus[i]);
//    addr += 2;
//  }
//  for (uint8_t i = 0; i < nrOfInputMenus; i++) {
//    EEPROM.put(addr, (void*)*inputMenus[i]);
//    addr += 2;
//  }
//  EEPROM.put(addr, (void*)menuGame);
//  addr += 2;
}

#endif
