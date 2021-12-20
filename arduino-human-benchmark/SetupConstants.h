#ifndef SETUPCONSTANTS_H
#define SETUPCONSTANTS_H

const uint8_t pinRS = 2;
const uint8_t pinEnable = 3;
const uint8_t pinD4 = 4;
const uint8_t pinD5 = 8;
const uint8_t pinD6 = 9;
const uint8_t pinD7 = 10;
const uint8_t pinContrast = 5;
const uint8_t pinBrightness = 6;

const uint8_t arrowNum = 0;
byte arrow[8] = {
  B10000,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B10000
};

const uint8_t pinJoyVx = A0;
const uint8_t pinJoyVy = A1;
const uint8_t pinJoyBttn = 7;

const uint8_t pinDIN = 12;
const uint8_t pinCS = 13;
const uint8_t pinCLK = 11;

#endif
