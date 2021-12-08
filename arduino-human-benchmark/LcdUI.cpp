#include "LcdUI.h"

// --------------------------------------------------------------
// ---------------- LcdText Function Definitions ----------------
// --------------------------------------------------------------
LcdText::LcdText(uint8_t col, uint8_t row, char* text, LcdMenu* targetMenu) {
    this->col = col;
    this->row = row;
    this->text = text;
    this->targetMenu = targetMenu;
}

void LcdText::center() {
    this->col = (lcdColNum - strlen(this->text)) / 2;
}

void LcdText::display(LiquidCrystal& lcd) {
    lcd.setCursor(this->col, this->row);
    lcd.print(this->text);
}

String LcdText::getText() {
  return this->text;
}

void LcdText::setText(char* text) {
  this->text = text;
}

LcdMenu* LcdText::getTargetMenu() {
    return this->targetMenu;
}

void LcdText::setTargetMenu(LcdMenu* targetMenu) {
  this->targetMenu = targetMenu;
}

// --------------------------------------------------------------
// ---------------- LcdMenu Function Definitions ----------------
// --------------------------------------------------------------
byte LcdMenu::arrowLeftNum;
byte LcdMenu::arrowRightNum;

LcdMenu::LcdMenu(LcdText* title, uint8_t nrOfOptions, LcdText* options, bool centered) {
    this->title = title;
    this->nrOfOptions = nrOfOptions;
    this->options = options;
    this->currentOption = 0;
    if (centered) {
        this->title->center();
        for (uint8_t i = 0; i < nrOfOptions; i++) {
            options[i].center();
        }
    }
}

void LcdMenu::setBackBttn(LcdText& backBttn) {
  this->options[nrOfOptions - 1] = backBttn;
}

void LcdMenu::display(LiquidCrystal& lcd) {
    lcd.clear();
    title->display(lcd);
    options[currentOption].display(lcd);
    // display left and right arrows on LCD for navigation
    lcd.setCursor(options[currentOption].col - 1, options[currentOption].row);
    lcd.write(byte(this->arrowLeftNum));
    lcd.setCursor(options[currentOption].col + strlen(options[currentOption].text), options[currentOption].row);
    lcd.write(byte(this->arrowRightNum));
}

LcdText LcdMenu::getCurrentOption() {
  return this->options[this->currentOption];
}

LcdMenu::~LcdMenu() {
    delete this->title;
    delete this->options;
}

// --------------------------------------------------------------
// ----------------- LcdNav Function Definitions ----------------
// --------------------------------------------------------------

LcdNav::LcdNav(LcdText* title, uint8_t nrOfOptions, LcdText* options, bool centered) : LcdMenu(title, nrOfOptions, options, centered) {}

String LcdNav::handleJoyMove(uint16_t x, bool& joyMoved, uint16_t minThresh, uint16_t maxThresh) {
    /*
        the return value is used to draw the menu only when the user completes a valid joystick move
        modifies the joyMoved variable declared in the main .ino file - this is needed because the method is called inside loop()
    */
    if (!joyMoved && x < minThresh) {
        joyMoved = true;
        this->currentOption--;
        if (this->currentOption < 0) {this->currentOption = this->nrOfOptions - 1;}
        return "moved";
    }
    if (!joyMoved && x > maxThresh) {
        joyMoved = true;
        this->currentOption++;
        if (this->currentOption == this->nrOfOptions) {this->currentOption = 0;}
        return "moved";
    }
    if (joyMoved && x >= minThresh && x <= maxThresh) {
        joyMoved = false;
    }
    return "idle";
}

LcdMenu* LcdNav::handleJoyPress(bool buttonState) {
  if (buttonState == LOW) {
    return this->options[this->currentOption].getTargetMenu();
  }
  return nullptr;
}

LcdNav::~LcdNav() {}
