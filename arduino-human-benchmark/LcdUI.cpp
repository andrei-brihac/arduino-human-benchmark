#include "LcdUI.h"

// --------------------------------------------------------------
// ---------------- LcdText Function Definitions ----------------
// --------------------------------------------------------------
LcdText::LcdText(String text) {
  this->text = text;
}

uint8_t LcdText::getCol() {
    return (lcdColNum - this->text.length()) / 2;
}

void LcdText::display(LiquidCrystal& lcd, uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
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
LcdButton::LcdButton(String text, LcdMenu* targetMenu) : LcdText(text) {
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
String LcdInputBox::alphabet = "abcdefghijklmnopqrstuvwxyz0123456789 ";

LcdInputBox::LcdInputBox(String text) : LcdText(text) {
  this->currChar = 0;
}

void LcdInputBox::setCurrCharLetter(char newChar) {
  this->text[currChar] = newChar;
}

char LcdInputBox::getCurrCharLetter() {
  return this->text[currChar];
}

char LcdInputBox::findNextCharInAlphabet(char c) {
  uint8_t alphabetIdx = LcdInputBox::alphabet.indexOf(c);
  alphabetIdx++;
  if (alphabetIdx == alphabet.length()) {
    alphabetIdx = 0;
  }
  return alphabet[alphabetIdx];
}

char LcdInputBox::findPrecedingCharInAlphabet(char c) {
  uint8_t alphabetIdx = LcdInputBox::alphabet.indexOf(c);
  alphabetIdx--;
  if (alphabetIdx < 0) {
    alphabetIdx = alphabet.length() - 1;
  }
  return alphabet[alphabetIdx];
}
// --------------------------------------------------------------
// ---------------- LcdMenu Function Definitions ----------------
// --------------------------------------------------------------
LcdMenu::LcdMenu(LcdText title, uint8_t type) {
  this->title = title;
  this->type = type;
}

uint8_t LcdMenu::getType() {
  return this->type;
}

void LcdMenu::setTitle(String newTitle) {
  this->title.setText(newTitle);
}

// --------------------------------------------------------------
// ----------------- LcdNav Function Definitions ----------------
// --------------------------------------------------------------
byte LcdNav::arrow;

LcdNav::LcdNav(LcdText title, uint8_t type, uint8_t nrOfOptions, LcdButton* options) : LcdMenu(title, type) {
  this->nrOfOptions = nrOfOptions;
  this->options = options;
  this->currentOption = 0;
}

LcdButton LcdNav::getCurrentOption() {
  return this->options[this->currentOption];
}

void LcdNav::initArrow(byte arrow) {
  LcdNav::arrow = arrow;
}

void LcdNav::setBackBttn(LcdButton backBttn) {
  this->options[nrOfOptions - 1] = backBttn;
}

void LcdNav::setBttnText(String text, short idx) {
  this->options[idx].setText(text);
}

void LcdNav::display(LiquidCrystal& lcd) {
  lcd.clear();
  this->title.display(lcd, this->title.getCol(), 0);
  this->options[currentOption].display(lcd, this->options[currentOption].getCol(), 1);
  uint8_t optionCol = this->options[currentOption].getCol();
  uint8_t optionLen = this->options[currentOption].getText().length();
  lcd.setCursor(optionCol - 1, 1);
  lcd.write(byte(this->arrow));
}

String LcdNav::handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh, uint16_t maxThresh) {
  /*
   *  the return value is used to draw the menu only when the user completes a valid joystick move
   *  modifies the joyMoved variable declared in the main .ino file - this is needed because the method is called inside loop()
   */
  if (!joyMoved && x < minThresh) {
      joyMoved = true;
      // change here depending on default orientation of joystick - here joystick is flipped 180 deg
      this->currentOption++;
      if (this->currentOption == this->nrOfOptions) {this->currentOption = 0;}
      return "moved";
  }
  if (!joyMoved && x > maxThresh) {
      joyMoved = true;
      // change here depending on default orientation of joystick - here joystick is flipped 180 deg
      this->currentOption--;
      if (this->currentOption < 0) {this->currentOption = this->nrOfOptions - 1;}
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

LcdInput::LcdInput(LcdText title, uint8_t type, LcdInputBox* input, LcdMenu* prevMenu, void (*func)(String)) : LcdMenu(title, type) {
  this->input = input;
  this->prevMenu = prevMenu;
  this->func = func;
}

void LcdInput::display(LiquidCrystal& lcd) {
  lcd.clear();
  this->title.display(lcd, this->title.getCol(), 0);
  this->input->display(lcd, this->input->getCol(), 1);
}

void LcdInput::blinkCursor(LiquidCrystal& lcd) {
  /*
   *   blinks the cursor at position currChar in the input string utilizing the timing static variables
   */
  uint8_t inputgetCol = this->input->getCol();
  uint8_t inputLength = this->input->getText().length();
  lcd.setCursor(inputgetCol + this->input->currChar, 1);
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
  uint16_t xDefault = 460;
  uint16_t yDefault = 460;
  uint16_t errorMargin = 100;
  // x-axis input for changing current character position
  if (y >= yDefault - errorMargin && y <= yDefault + errorMargin) {
    if (!joyMoved && x < minThresh) {
        joyMoved = true;
        // change here depending on default orientation of joystick - here joystick is flipped 180 deg
        this->input->currChar++;
        if (this->input->currChar == this->input->getText().length()) {this->input->currChar = 0;}
        return "moved";
    }
    if (!joyMoved && x > maxThresh) {
        joyMoved = true;
        // change here depending on default orientation of joystick - here joystick is flipped 180 deg
        this->input->currChar--;
        if (this->input->currChar < 0) {this->input->currChar = this->input->getText().length() - 1;}
        return "moved";
    }
    if (joyMoved && x >= minThresh && x <= maxThresh) {
      joyMoved = false;
    }
  }
  // y-axis input for changing current character value
  if (x >= xDefault - errorMargin && x <= xDefault + errorMargin) {
    if (!joyMoved && y < minThresh) {
        joyMoved = true;
        // change here depending on default orientation of joystick - here joystick is flipped 180 deg
        // go to next char in alphabet of current char
        char nextChar = LcdInputBox::findNextCharInAlphabet(this->input->getCurrCharLetter());
        this->input->setCurrCharLetter(nextChar);
        return "moved";
    }
    if (!joyMoved && y > maxThresh) {
        joyMoved = true;
        // change here depending on default orientation of joystick - here joystick is flipped 180 deg
        // go to preceding char in alphabet of current char
        char precedingChar = LcdInputBox::findPrecedingCharInAlphabet(this->input->getCurrCharLetter());
        this->input->setCurrCharLetter(precedingChar);
        return "moved";
    }
    if (joyMoved && y >= minThresh && y <= maxThresh) {
        joyMoved = false;
    }
  }
  return "idle";
}

LcdMenu* LcdInput::handleJoyPress(bool buttonState) {
  if (buttonState == LOW) {
    func(this->input->getText());
    return prevMenu;
  }
  return nullptr;
}

void LcdInput::setPrevMenu(LcdMenu* prevMenu) {
  this->prevMenu = prevMenu;
}

LcdInput::~LcdInput() {
  delete this->input;
}

// --------------------------------------------------------------
// ---------------- LcdGame Function Definitions ----------------
// --------------------------------------------------------------
LcdGame::LcdGame(LcdText title, uint8_t type) : LcdMenu(title, type) {
  this->score = -1;
  this->lives = -1;
  this->level = -1;
}

bool LcdGame::setVariables(int score, uint8_t lives, uint8_t level) {
  // returns true when any old value is replaced, false otherwise - is used in main to draw the menu on change
  bool changed = score != this->score || lives != this->lives || level != this->level;
  this->score = score;
  this->lives = lives;
  this->level = level;
  return changed;
}

void LcdGame::display(LiquidCrystal& lcd) {
  lcd.clear();
  this->title.display(lcd, this->title.getCol(), 0);
  char str[17];
  snprintf(str, sizeof(str), "S:%d   L:%d Lv:%d", this->score, this->lives, this->level);
  lcd.setCursor(1, 1);
  lcd.print(str);
}
