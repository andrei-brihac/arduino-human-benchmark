#include <Arduino.h>
#include <LiquidCrystal.h>
#include "LcdUI.h"
#include "SetupConstants.h"
#include "Menus.h"

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);
uint16_t lcdContrast = 350;

LcdMenu* currentMenu;
uint8_t currentMenuType;
unsigned long long lastMenuChange;
uint16_t menuChangeDelay = 200;

uint16_t joyVxRead;
uint16_t joyVyRead;
bool joyMoved = false;

void setup() {
  pinMode(pinContrast, OUTPUT);
  analogWrite(pinContrast, lcdContrast);

  pinMode(pinJoyVx, INPUT);
  pinMode(pinJoyVy, INPUT);
  pinMode(pinJoyBttn, INPUT_PULLUP);

  lcd.createChar(arrowLeftNum, arrowLeft);
  lcd.createChar(arrowRightNum, arrowRight);
  lcd.begin(lcdColNum, lcdRowNum);
  LcdMenu::arrowLeftNum = arrowLeftNum;
  LcdMenu::arrowRightNum = arrowRightNum;
  
  initMenus();
  currentMenu = menuMain;
  currentMenuType = MENU_TYPES::NAV;
  currentMenu->display(lcd);

  Serial.begin(9600);
}

void loop() {
  handleLcdMenu();
}

void handleLcdMenu() {
  if (currentMenuType == MENU_TYPES::NAV) {
    String joyState = currentMenu->handleJoyMove(analogRead(pinJoyVy), joyMoved);
    if (joyState == "moved") {
      currentMenu->display(lcd);
    }
    LcdMenu* newMenu = currentMenu->handleJoyPress(digitalRead(pinJoyBttn));
    if (newMenu && millis() - lastMenuChange > menuChangeDelay) {
      currentMenu = newMenu;
      currentMenu->display(lcd);
      lastMenuChange = millis();
    }
  }
  if (currentMenuType == MENU_TYPES::IN) {
    // do something
  }
  if (currentMenuType == MENU_TYPES::GAME) {
    // do something
  }
}
