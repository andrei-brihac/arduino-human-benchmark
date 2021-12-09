#include <Arduino.h>
#include <LiquidCrystal.h>
#include "LcdUI.h"
#include "SetupConstants.h"
#include "Menus.h"

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);
LcdMenu* currentMenu;
bool drawMenu = false;
unsigned long long lastMenuChange;
uint16_t menuChangeDelay = 200;

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
  LcdNav::initArrows(arrowLeftNum, arrowRightNum);
  
  initMenus();
  currentMenu = menuMain;
  currentMenu->display(lcd);

  Serial.begin(9600);
}

void loop() {
  handleLcdMenu();
}

void handleLcdMenu() {
  // returns 'moved' only when the joyMoved variable transitions from false to true --> this is a signal to draw the modified menu
  uint16_t vxRead = analogRead(pinJoyVx);
  uint16_t vyRead = analogRead(pinJoyVy);
  String joyState = currentMenu->handleJoyMove(vyRead, vxRead, joyMoved);  // inversed reads because of the way I'm holding the joystick
  if (joyState == "moved") {
      drawMenu = true;
  }
  // returns null if button is not pressed or the current menu button doesn't do anything
  // returns a pointer to the chosen menu when the button is pressed
  // so when the pointer is not null it's a signal that a new menu must be drawn
  bool buttonState = digitalRead(pinJoyBttn);
  if (millis() - lastMenuChange > menuChangeDelay) {  // look for button input a short delay after changing menus
    LcdMenu* newMenu = currentMenu->handleJoyPress(buttonState);
    if (newMenu) {
        currentMenu = newMenu;
        drawMenu = true;
        lastMenuChange = millis();
    }
  }
  // didn't manage to use typeid correctly, so I'm just saving the menu type inside each object - 1 byte of memory is lost :(
  uint8_t currentMenuType = currentMenu->getType();
  if (currentMenuType == MENU_TYPES::NAV) {
    lcd.noBlink();  // make sure the cursor isn't blinking after exiting an input menu
  }
  if (currentMenuType == MENU_TYPES::IN) {
    ((LcdInput*)currentMenu)->blinkCursor(lcd);  // blink the cursor at the chosen location
  }
  if (currentMenuType == MENU_TYPES::GAME) {
    // do something
  }
  // if a signal to draw the menu has been received from moving or pressing the joystick, then we draw it and signal that it has been drawn
  if (drawMenu) {
    currentMenu->display(lcd);
    analogWrite(pinContrast, lcdContrast);
    drawMenu = false;
  }
}
