#ifndef MENUVARIABLES_H
#define MENUVARIABLES_H

#include <Arduino.h>

uint16_t lcdContrast = 350;

void setLcdContrast(String value) {
    lcdContrast = value.toInt();
}

#endif
