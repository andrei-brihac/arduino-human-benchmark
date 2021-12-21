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
LcdButton bttnMainMenu = LcdButton("Main Menu");
// nav menus
const uint8_t nrOfNavMenus = 4;
LcdMenu *menuSettings, *menuUser, *menuScore, *menuAbout;
LcdMenu** navMenus[nrOfNavMenus] = {&menuSettings, &menuUser, &menuScore, &menuAbout};
// input menus
LcdMenu *menuContrastLcd, *menuUserSet, *menuBrightnessLcd, *menuBrightnessLed;
// game menu
LcdMenu* menuGame;
// check each menu class constructor to see how to declare a menu

void initMenus() {
  // Any input menu is instantiated before the nav menu containing it, making the linking easier.
  menuGame = new LcdGame(LcdText("Game Menu"), MENU_TYPES::GAME);
  // ------------------------------------------
  menuScore = new LcdNav(
    LcdText("Score"),
    MENU_TYPES::NAV,
    2,
    new LcdButton[2]{
      LcdButton("High Score"),
      bttnMainMenu
    }
  );
  // --------------------------------------------
  menuUserSet = new LcdInput(
    LcdText("Set Username"),
    MENU_TYPES::IN,
    new LcdInputBox(userName.value),
    menuUser,
    &setUserName
  );
  menuUser = new LcdNav(
    LcdText("User Settings"),
    MENU_TYPES::NAV,
    2,
    new LcdButton[2]{
      LcdButton("Set Username", menuUserSet),
      bttnMainMenu
    }
  );
  // Linking input menus back to the user menu.
  ((LcdInput*)menuUserSet)->setPrevMenu(menuUser);
  // --------------------------------------------
  menuContrastLcd = new LcdInput(
    LcdText("LCD Contrast"),
    MENU_TYPES::IN,
    new LcdInputBox(String(lcdContrast.value)),
    menuSettings,
    &setLcdContrast
  );
  menuBrightnessLcd = new LcdInput(
    LcdText("LCD Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(lcdBrightness.value),
    menuSettings,
    &setLcdBrightness
  );
  menuBrightnessLed = new LcdInput(
    LcdText("LED Brightness"),
    MENU_TYPES::IN,
    new LcdInputBox(ledBrightness.value),
    menuSettings,
    &setLedBrightness
  );
  menuSettings = new LcdNav(
    LcdText("Settings"),
    MENU_TYPES::NAV,
    4,
    new LcdButton[4]{
      LcdButton("LCD Contrast", menuContrastLcd),
      LcdButton("LCD Brightness", menuBrightnessLcd),
      LcdButton("LED Brightness", menuBrightnessLed),
      bttnMainMenu
    }
  );
  // Linking input menus back to the settings menu.
  ((LcdInput*)menuContrastLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLcd)->setPrevMenu(menuSettings);
  ((LcdInput*)menuBrightnessLed)->setPrevMenu(menuSettings);
  // --------------------------------------------
  menuAbout = new LcdNav(
    LcdText("About"),
    MENU_TYPES::NAV,
    4,
    new LcdButton[4]{
      LcdButton("HumanBenchmark"),
      LcdButton("BrihacAndrei"),
      LcdButton("git:todo"),
      bttnMainMenu
    }
  );
  // --------------------------------------------
  // Main menu is instantiated last because it needs to be linked to the previously instantiated menus.
  menuMain = new LcdNav(
    LcdText("Main Menu"),
    MENU_TYPES::NAV,
    5,
    new LcdButton[5]{
      LcdButton("Start Game", menuGame),
      LcdButton("Score", menuScore),
      LcdButton("User Menu", menuUser),
      LcdButton("Settings", menuSettings),
      LcdButton("About", menuAbout)
      }
  );
  bttnMainMenu.setTargetMenu(menuMain);
  // Linking each back button to the main menu.
  for (uint8_t i = 0; i < nrOfNavMenus; i++) {
    ((LcdNav*)(*navMenus[i]))->setBackBttn(bttnMainMenu);
  }
}

#endif
