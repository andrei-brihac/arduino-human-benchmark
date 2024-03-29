#ifndef LCDUI_H
#define LCDUI_H

#include <Arduino.h>
#include <LiquidCrystal.h>

const uint8_t lcdColNum = 16;
const uint8_t lcdRowNum = 2;

class LcdText;
class LcdButton;
class LcdMenu;
class LcdNav;
class LcdInput;

class LcdText {
  private:
    String text;
  public:
    LcdText(String text="");
    uint8_t getCol();
    void display(LiquidCrystal& lcd, uint8_t col, uint8_t row);
    String getText();
    void setText(String text);
  friend class LcdInputBox;
};

class LcdButton : public LcdText {
  private:
    LcdMenu* targetMenu;
  public:
    LcdButton(String text, LcdMenu* targetMenu=nullptr);
    LcdMenu* getTargetMenu();
    void setTargetMenu(LcdMenu* targetMenu);
};

class LcdInputBox : public LcdText {
  static String alphabet;
  private:
    short currChar;
  public:
    LcdInputBox(String text);
    char getCurrCharLetter();
    void setCurrCharLetter(char newChar);
    static char findNextCharInAlphabet(char c);
    static char findPrecedingCharInAlphabet(char c);
  friend class LcdInput;
};

class LcdMenu {
  private:
    LcdText title;
    uint8_t type;
  public:
    LcdMenu(LcdText title, uint8_t type=0);
    uint8_t getType();
    void setTitle(String newTitle="");
    virtual void display(LiquidCrystal& lcd) = 0;
    virtual String handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750) = 0;
    virtual LcdMenu* handleJoyPress(bool buttonState) = 0;
  friend class LcdNav;
  friend class LcdInput;
  friend class LcdGame;
};

class LcdNav : public LcdMenu {
  private:
    uint8_t nrOfOptions;
    LcdButton* options;
    short currentOption;
    static byte arrow;
  public:
    LcdNav(LcdText title, uint8_t type=0, uint8_t nrOfOptions=0, LcdButton* options=nullptr);
    void setBackBttn(LcdButton backBttn);
    void setBttnText(String text, short idx);
    static void initArrow(byte arrow);
    LcdButton getCurrentOption();
    void display(LiquidCrystal& lcd);
    String handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750);
    LcdMenu* handleJoyPress(bool buttonState);
    ~LcdNav();
};

class LcdInput : public LcdMenu {
  static uint16_t blinkDelay;
  static unsigned long long lastBlinked;
  static bool blinked;
  private:
    LcdInputBox* input;
    LcdMenu* prevMenu;
    void (*func)(String);
  public:
    LcdInput(LcdText title, uint8_t type=1, LcdInputBox* input=nullptr, LcdMenu* prevMenu=nullptr, void (*func)(String)=nullptr);
    void display(LiquidCrystal& lcd);
    String handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750);
    LcdMenu* handleJoyPress(bool buttonState);
    void blinkCursor(LiquidCrystal& lcd);
    void setPrevMenu(LcdMenu* prevMenu);
    ~LcdInput();
};

class LcdGame : public LcdMenu {
  private:
    int score;
    uint8_t lives;
    uint8_t level;
  public:
    LcdGame(LcdText title, uint8_t type=2);
    void display(LiquidCrystal& lcd);
    String handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750) {return "idle";}
    LcdMenu* handleJoyPress(bool buttonState) {return nullptr;}
    bool setVariables(int score, uint8_t lives, uint8_t level);
};

#endif
