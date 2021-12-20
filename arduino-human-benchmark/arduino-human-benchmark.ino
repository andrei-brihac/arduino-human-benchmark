#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LedControl.h>
#include "SetupConstants.h"
#include "Menu.h"
#include "Game.h"

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);
LcdMenu* currentMenu;
bool drawMenu = false;
unsigned long long lastMenuChange;
uint16_t menuChangeDelay = 500;

bool joyMoved = false;

LedControl ledControl = LedControl(pinDIN, pinCLK, pinCS, 1);
Game game;
bool inGame = false;

void setup() {
  Serial.begin(9600);
  
  readControlVariables();
  pinMode(pinContrast, OUTPUT);
  pinMode(pinBrightness, OUTPUT);
  analogWrite(pinContrast, lcdContrast.value);
  analogWrite(pinBrightness, lcdBrightness.value.toInt());

  pinMode(pinJoyVx, INPUT);
  pinMode(pinJoyVy, INPUT);
  pinMode(pinJoyBttn, INPUT_PULLUP);

  lcd.createChar(arrowNum, arrow);
  lcd.begin(lcdColNum, lcdRowNum);
  LcdNav::initArrow(arrowNum);

  initMenus();
  currentMenu = menuMain;
  currentMenu->display(lcd);

  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, ledBrightness.value.toInt());
  randomSeed(analogRead(0));
}

void loop() {
  handleLcdMenu();
  handleGame();
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
    inGame = true;
    drawMenu = ((LcdGame*)currentMenu)->setVariables(game.getScore(), game.getLives(), game.getLevel());  // this method returns true when any of the variables change value
  }
  // if a signal to draw the menu has been received from moving or pressing the joystick, then we draw it and signal that it has been drawn
  if (drawMenu) {
    currentMenu->display(lcd);
    analogWrite(pinContrast, lcdContrast.value);
    analogWrite(pinBrightness, lcdBrightness.value.toInt());
    ledControl.setIntensity(0, ledBrightness.value.toInt());
    drawMenu = false;
  }
}

void handleGame() {
  if (!inGame) {return;}
  game.displayGame(ledControl);
}
