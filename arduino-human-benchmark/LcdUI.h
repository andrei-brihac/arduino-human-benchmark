#ifndef LCDUI_H
#define LCDUI_H

#include <Arduino.h>
#include <LiquidCrystal.h>

const uint8_t lcdColNum = 16;
const uint8_t lcdRowNum = 2;

class LcdText;
class LcdMenu;

class LcdText {
  private:
    uint8_t col;
    uint8_t row;
    String text;
  public:
    LcdText(uint8_t col, uint8_t row, String text);
    void centerText();
    void displayText(LiquidCrystal lcd);
  friend class LcdMenu;
};

class LcdMenu {
  private:
    LcdText* title;
    uint8_t nrOfOptions;
    LcdText* options;
    uint8_t currentOption;
  public:
    static byte arrowLeftNum;
    static byte arrowRightNum;
    LcdMenu(LcdText* title, uint8_t nrOfOptions, LcdText* options, bool centered=true);
    static void initArrows();
    void displayMenu(LiquidCrystal lcd);
    ~LcdMenu();
};

#endif