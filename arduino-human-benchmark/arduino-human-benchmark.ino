#include <Arduino.h>
#include <LiquidCrystal.h>
#include "SetupConstants.h"
#include "LcdUI.h"

// --------------------------------------------------------------
// -------------------- LCD Menu Definitions --------------------
// --------------------------------------------------------------
LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);
const uint8_t nrOfMenus = 3;
uint8_t currentMenu = 0;
LcdMenu mainMenu(new LcdText(0, 0, "Main Menu"), 3, new LcdText[3]{LcdText(0, 1, "Start Game"), LcdText(0, 1, "User Menu"), LcdText(0, 1, "Score")});
LcdMenu userMenu(new LcdText(0, 0, "User Settings"), 2, new LcdText[2]{LcdText(0, 1, "Create User"), LcdText(0, 1, "Delete User")});
LcdMenu scoreMenu(new LcdText(0, 0, "Score"), 2, new LcdText[2]{LcdText(0, 1, "High Score"), LcdText(0, 1, "Reset")});
LcdMenu menus[nrOfMenus] = {mainMenu, userMenu, scoreMenu};

void setup() {
  lcd.createChar(arrowLeftNum, arrowLeft);
  lcd.createChar(arrowRightNum, arrowRight);
  LcdMenu::arrowLeftNum = arrowLeftNum;
  LcdMenu::arrowRightNum = arrowRightNum;
  lcd.begin(lcdColNum, lcdRowNum);
  pinMode(pinContrast, OUTPUT);
  menus[currentMenu].displayMenu(lcd);
  analogWrite(pinContrast, 350);
}

void loop() {

}