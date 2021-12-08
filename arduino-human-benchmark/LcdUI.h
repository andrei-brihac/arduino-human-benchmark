#ifndef LCDUI_H
#define LCDUI_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>

const uint8_t lcdColNum = 16;
const uint8_t lcdRowNum = 2;

class LcdText;
class LcdMenu;
class LcdNav;

class LcdText {
  private:
    uint8_t col;
    uint8_t row;
    char* text;
    LcdMenu* targetMenu;
  public:
    LcdText(uint8_t col, uint8_t row, char* text, LcdMenu* targetMenu=nullptr);
    void center();
    void display(LiquidCrystal& lcd);
    String getText();
    void setText(char* text);
    LcdMenu* getTargetMenu();
    void setTargetMenu(LcdMenu* targetMenu);
  friend class LcdMenu;
};

class LcdMenu {
  public:
    LcdText* title;
    uint8_t nrOfOptions;
    LcdText* options;
    short currentOption;
  public:
    static byte arrowLeftNum;
    static byte arrowRightNum;
    LcdMenu(LcdText* title=nullptr, uint8_t nrOfOptions=0, LcdText* options=nullptr, bool centered=true);
    void setBackBttn(LcdText& backBttn);
    void display(LiquidCrystal& lcd);
    LcdText getCurrentOption();
    virtual String handleJoyMove(uint16_t x, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750) = 0;
    virtual LcdMenu* handleJoyPress(bool buttonState) = 0;
    ~LcdMenu();
  friend class LcdNav;
};

class LcdNav : public LcdMenu {
  public:
    LcdNav(LcdText* title=nullptr, uint8_t nrOfOptions=0, LcdText* options=nullptr, bool centered=true);
    String handleJoyMove(uint16_t x, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750);
    LcdMenu* handleJoyPress(bool buttonState);
    ~LcdNav();
};

#endif
