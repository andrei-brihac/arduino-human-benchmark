#ifndef MENUS_H
#define MENUS_H

#include <EEPROM.h>
#include "LcdUI.h"
#include "ControlVariables.h"

enum MENU_TYPES {
  NAV, IN, GAME
};

// main menu
LcdMenu* menuMain;
LcdButton* bttnMainMenu = new LcdButton("Main Menu");
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
  // Any input menu is instantiated before the nav menu containing it, making the linking easier.
  menuGame = new LcdGame(new LcdText("Game Menu"), MENU_TYPES::GAME);
  // ------------------------------------------
  menuScore = new LcdNav(
    new LcdText("Score"),
    MENU_TYPES::NAV,
    3,
    new LcdButton*[3]{
      new LcdButton("High Score"),
      new LcdButton("Reset"),
      bttnMainMenu
    }
  );
  // --------------------------------------------
  menuUserSet = new LcdInput(
    new LcdText("Set Username"),
    MENU_TYPES::IN,
    new LcdInputBox(userName.value),
    menuUser,
    &setUserName
  );
  menuUser = new LcdNav(
    new LcdText("User Settings"),
    MENU_TYPES::NAV,
    3,
    new LcdButton*[3]{
      new LcdButton("Set Username", menuUserSet),
      new LcdButton("Delete User"),
      bttnMainMenu
    }
  );
  // Linking input menus back to the user menu.
  ((LcdInput*)menuUserSet)->setPrevMenu(menuUser);
  // --------------------------------------------
  menuContrastLcd = new LcdInput(
    new LcdText("LCD Contrast"),
    MENU_TYPES::IN,
    new LcdInputBox(String(lcdContrast.value)),
    menuSettings,
    &setLcdContrast
  );
  menuBrightnessLcd = new LcdInput(
    new LcdText("LCD Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(lcdBrightness.value),
    menuSettings,
    &setLcdBrightness
  );
  menuBrightnessLed = new LcdInput(
    new LcdText("LED Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(ledBrightness.value),
    menuSettings,
    &setLedBrightness
  );
  menuSettings = new LcdNav(
    new LcdText("Settings"),
    MENU_TYPES::NAV,
    4,
    new LcdButton*[4]{
      new LcdButton("LCD Contrast", menuContrastLcd),
      new LcdButton("LCD Brightness", menuBrightnessLcd),
      new LcdButton("LED Brightness", menuBrightnessLed),
      bttnMainMenu
    }
  );
  // Linking input menus back to the settings menu.
  ((LcdInput*)menuContrastLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLed)->setPrevMenu(menuSettings);
  // --------------------------------------------
  menuAbout = new LcdNav(
    new LcdText("About"),
    MENU_TYPES::NAV,
    4,
    new LcdButton*[4]{
      new LcdButton("HumanBenchmark"),
      new LcdButton("BrihacAndrei"),
      new LcdButton("git:todo"),
      bttnMainMenu
    }
  );
  // --------------------------------------------
  // Main menu is instantiated last because it needs to be linked to the previously instantiated menus.
  menuMain = new LcdNav(
    new LcdText("Main Menu"),
    MENU_TYPES::NAV,
    5,
    new LcdButton*[5]{
      new LcdButton("Start Game", menuGame),
      new LcdButton("Score", menuScore),
      new LcdButton("User Menu", menuUser),
      new LcdButton("Settings", menuSettings),
      new LcdButton("About", menuAbout)
      }
  );
  bttnMainMenu->setTargetMenu(menuMain);
  // Linking each back button to the main menu.
  for (uint8_t i = 0; i < nrOfNavMenus; i++) {
    ((LcdNav*)(*navMenus[i]))->setBackBttn(bttnMainMenu);
  }
}

#endif
