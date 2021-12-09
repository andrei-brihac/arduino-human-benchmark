#include "LcdUI.h"

// --------------------------------------------------------------
// ---------------- LcdText Function Definitions ----------------
// --------------------------------------------------------------
LcdText::LcdText(uint8_t col, uint8_t row, String text) {
  this->col = col;
  this->row = row;
  this->text = text;
}

void LcdText::center() {
    this->col = (lcdColNum - this->text.length()) / 2;
}

void LcdText::display(LiquidCrystal& lcd) {
    lcd.setCursor(this->col, this->row);
    lcd.print(this->text);
}

String LcdText::getText() {
  return this->text;
}

void LcdText::setText(String text) {
  this->text = text;
}

// --------------------------------------------------------------
// ---------------- LcdButton Function Definitions --------------
// --------------------------------------------------------------
LcdButton::LcdButton(uint8_t col, uint8_t row, String text, LcdMenu* targetMenu) : LcdText(col, row, text) {
  this->targetMenu = targetMenu;
}

LcdMenu* LcdButton::getTargetMenu() {
  return this->targetMenu;
}

void LcdButton::setTargetMenu(LcdMenu* targetMenu) {
  this->targetMenu = targetMenu;
}

// --------------------------------------------------------------
// -------------- LcdInputBox Function Definitions --------------
// --------------------------------------------------------------
LcdInputBox::LcdInputBox(uint8_t col, uint8_t row, String text) : LcdText(col, row, text) {
  this->center();
  this->currChar = 0;
}
// --------------------------------------------------------------
// ---------------- LcdMenu Function Definitions ----------------
// --------------------------------------------------------------
LcdMenu::LcdMenu(LcdText* title, uint8_t type) {
  this->title = title;
  this->title->center();
  this->type = type;
}

uint8_t LcdMenu::getType() {
  return this->type;
}

LcdMenu::~LcdMenu() {
  delete this->title;
}

// --------------------------------------------------------------
// ----------------- LcdNav Function Definitions ----------------
// --------------------------------------------------------------
byte LcdNav::arrowLeftNum;
byte LcdNav::arrowRightNum;

LcdNav::LcdNav(LcdText* title, uint8_t type, uint8_t nrOfOptions, LcdButton* options, bool centered) : LcdMenu(title, type) {
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

void LcdNav::initArrows(byte arrowLeftNum, byte arrowRightNum) {
  LcdNav::arrowLeftNum = arrowLeftNum;
  LcdNav::arrowRightNum = arrowRightNum;
}

LcdButton LcdNav::getCurrentOption() {
  return this->options[this->currentOption];
}

void LcdNav::setBackBttn(LcdButton& backBttn) {
  this->options[nrOfOptions - 1] = backBttn;
}

void LcdNav::display(LiquidCrystal& lcd) {
  lcd.clear();
  this->title->display(lcd);
  this->options[currentOption].display(lcd);
  // display left and right arrows on LCD for navigation
  lcd.setCursor(this->options[currentOption].col - 1, this->options[currentOption].row);
  lcd.write(byte(this->arrowLeftNum));
  lcd.setCursor(this->options[currentOption].col + this->options[currentOption].text.length(), this->options[currentOption].row);
  lcd.write(byte(this->arrowRightNum));
}

String LcdNav::handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh, uint16_t maxThresh) {
  /*
   *  the return value is used to draw the menu only when the user completes a valid joystick move
   *  modifies the joyMoved variable declared in the main .ino file - this is needed because the method is called inside loop()
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

LcdNav::~LcdNav() {
  delete this->options;
}

// --------------------------------------------------------------
// --------------- LcdInput Function Definitions ----------------
// --------------------------------------------------------------
uint16_t LcdInput::blinkDelay = 500;
unsigned long long LcdInput::lastBlinked = 0;
bool LcdInput::blinked = false;

LcdInput::LcdInput(LcdText* title, uint8_t type, LcdInputBox* input, LcdMenu* prevMenu) : LcdMenu(title, type) {
  this->input = input;
  this->prevMenu = prevMenu;
}

void LcdInput::display(LiquidCrystal& lcd) {
  lcd.clear();
  this->title->display(lcd);
  this->input->display(lcd);
}

void LcdInput::blinkCursor(LiquidCrystal& lcd) {
  /*
   *   blinks the cursor at position currChar in the input string utilizing the timing static variables
   */
  lcd.setCursor(this->input->col + this->input->currChar, this->input->row);
  if (millis() - lastBlinked > blinkDelay) {
    if (!blinked) {lcd.blink(); lastBlinked = millis(); blinked=true;}
    else {lcd.noBlink(); blinked=false;}
  }
}

String LcdInput::handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh, uint16_t maxThresh) {
  /*
   *  the return value is used to draw the menu only when the user completes a valid joystick move
   *  modifies the joyMoved variable declared in the main .ino file - this is needed because the method is called inside loop()
   */
  if (!joyMoved && x < minThresh) {
      joyMoved = true;
      this->input->currChar--;
      if (this->input->currChar < 0) {this->input->currChar = this->input->getText().length() - 1;}
      return "moved";
  }
  if (!joyMoved && x > maxThresh) {
      joyMoved = true;
      this->input->currChar++;
      if (this->input->currChar == this->input->getText().length()) {this->input->currChar = 0;}
      return "moved";
  }
  if (joyMoved && x >= minThresh && x <= maxThresh) {
      joyMoved = false;
  }
  return "idle";
}

LcdMenu* LcdInput::handleJoyPress(bool buttonState) {
  if (buttonState == LOW) {return prevMenu;}
  return nullptr;
}

void LcdInput::setPrevMenu(LcdMenu* prevMenu) {
  this->prevMenu = prevMenu;
}
