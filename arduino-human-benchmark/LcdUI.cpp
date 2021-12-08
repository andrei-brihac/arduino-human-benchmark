#include "LcdUI.h"

// --------------------------------------------------------------
// ---------------- LcdText Function Definitions ----------------
// --------------------------------------------------------------
LcdText::LcdText(uint8_t col, uint8_t row, String text) {
    this->col = col;
    this->row = row;
    this->text = text;
}

void LcdText::centerText() {
    this->col = (lcdColNum - this->text.length()) / 2;
}

void LcdText::displayText(LiquidCrystal lcd) {
    lcd.setCursor(this->col, this->row);
    lcd.print(this->text);
}

// --------------------------------------------------------------
// ---------------- LcdMenu Function Definitions ----------------
// --------------------------------------------------------------
byte LcdMenu::arrowLeftNum = -1;
byte LcdMenu::arrowRightNum = -1;

LcdMenu::LcdMenu(LcdText* title, uint8_t nrOfOptions, LcdText* options, bool centered) {
    this->title = title;
    this->nrOfOptions = nrOfOptions;
    this->options = options;
    this->currentOption = 0;
    if (centered) {
        this->title->centerText();
        for (uint8_t i = 0; i < nrOfOptions; i++) {
            options[i].centerText();
        }
    }
}

void LcdMenu::displayMenu(LiquidCrystal lcd) {
    title->displayText(lcd);
    options[currentOption].displayText(lcd);
    // display left and right arrows on LCD for navigation
    lcd.setCursor(options[currentOption].col - 2, options[currentOption].row);
    lcd.write(byte(this->arrowLeftNum));
    lcd.setCursor(options[currentOption].col + options[currentOption].text.length() + 1, options[currentOption].row);
    lcd.write(byte(this->arrowRightNum));
}

LcdMenu::~LcdMenu() {
    delete this->title;
    delete this->options;
}