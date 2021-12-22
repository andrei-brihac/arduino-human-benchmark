#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

#include <Arduino.h>
#include <EEPROM.h>
#include <string.h>

int eeAddr = 0;

template <class T>
class eeVar {
  public:
    T value;
    int addr;
    eeVar(T value, int addr) {
      this->value = value;
      this->addr = addr;
      eeAddr += sizeof(this->value);
    }
    void readVal() {
      EEPROM.get(this->addr, this->value);
    }
    void saveVal() {
      EEPROM.put(this->addr, this->value);
    }
    void setVal(T value) {
      this->value = value;
    }
};

template<> eeVar<String>::eeVar(String value, int addr) {
  this->value = value;
  this->addr = addr;
  eeAddr += sizeof(this->value) + 1;
}

template<> void eeVar<String>::saveVal() {
  EEPROM.write(this->addr, this->value.length());
  for (uint8_t i = 0; i < this->value.length(); i++) {
    EEPROM.write(this->addr + 1 + i, this->value[i]);
  }
}

template<> void eeVar<String>::readVal() {
  int strLen = EEPROM.read(this->addr);
  char str[strLen + 1];
  for (uint8_t i = 0; i < strLen; i++) {
    str[i] = EEPROM.read(this->addr + 1 + i);
  }
  str[strLen] = '\0';
  this->value = String(str);
}

eeVar<int> lcdContrast(350, eeAddr);
eeVar<int> highScore(0, eeAddr);
eeVar<String> lcdBrightness("255", eeAddr);
eeVar<String> ledBrightness("05", eeAddr);
eeVar<String> userName("        ", eeAddr);
eeVar<String> highScoreUserName("        ", eeAddr);

void clearEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

void readControlVariables() {
  lcdContrast.readVal();
  lcdBrightness.readVal();
  ledBrightness.readVal();
  userName.readVal();
  highScore.readVal();
  highScoreUserName.readVal();
}

void setLcdContrast(String newContrast) {
    lcdContrast.setVal(newContrast.toInt());
    lcdContrast.saveVal();
}

void setUserName(String newUserName) {
  userName.setVal(newUserName);
  userName.saveVal();
}

void setLcdBrightness(String newBrightness) {
  lcdBrightness.setVal(newBrightness);
  lcdBrightness.saveVal();
}

void setLedBrightness(String newBrightness) {
  ledBrightness.setVal(newBrightness);
  ledBrightness.saveVal();
}

#endif
