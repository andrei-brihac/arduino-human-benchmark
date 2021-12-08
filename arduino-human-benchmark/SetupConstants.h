#ifndef SETUPCONSTANTS_H
#define SETUPCONSTANTS_H

const uint8_t pinRS = 2;
const uint8_t pinEnable = 3;
const uint8_t pinD4 = 9;
const uint8_t pinD5 = 8;
const uint8_t pinD6 = 13;
const uint8_t pinD7 = 5;
const uint8_t pinContrast = 6;

const uint8_t arrowLeftNum = 0;
const uint8_t arrowRightNum = 1;
byte arrowLeft[8] = {
  B00001,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010,
  B00001
};
byte arrowRight[8] = {
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

#endif
