#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

#include <Arduino.h>

String lcdContrast = "350";
String userName = "              ";

void setLcdContrast(String newContrast) {
    lcdContrast = newContrast;
}

void setUserName(String newUserName) {
  newUserName.trim();
  userName = newUserName;
}

#endif
