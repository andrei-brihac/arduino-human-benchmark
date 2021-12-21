#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 8
#define SQUARE_NUMBER 9

#include <Arduino.h>
#include <LedControl.h>

enum GAME_STATES {
  SELECTION, RANDOMIZE, SHOW
};

const uint16_t selectionBlinkInterval = 500;
const uint16_t showBlinkInterval = 1000;

class LedSquare {
  private:
    uint8_t row;
    uint8_t col;
    static const uint8_t width = 2;
    static const uint8_t height = 2;
  public:
    LedSquare(uint8_t row=0, uint8_t col=0) {
      this->row = row;
      this->col = col;
    }
    void setState(bool matrix[MATRIX_SIZE][MATRIX_SIZE], bool state) {
      for (uint8_t i = row; i < row + width; i++) {
        for (uint8_t j = col; j < col + height; j++) {
          matrix[i][j] = state;
        }
      }
    }
    bool shouldBlink(uint16_t blinkInterval) {
      return millis() % blinkInterval > blinkInterval / 2 ;
    }
};

class Game {
  private:
    int score;
    uint8_t lives;
    uint8_t level;
    short currSqrRow;
    short currSqrCol;
    bool currSqrState;
  public:
    static bool matrix[MATRIX_SIZE][MATRIX_SIZE];
    static LedSquare squares[SQUARE_NUMBER/3][SQUARE_NUMBER/3];
    Game();
    int getScore() {return score;}
    uint8_t getLives() {return lives;}
    uint8_t getLevel() {return level;}
    void setCurrSquare(short sqrRow, short sqrCol) {currSqrRow = sqrRow; currSqrCol = sqrCol;}
    void displayGame(LedControl& lc);
    void blinkCurrSquare(uint16_t blinkInterval);
    void wipeSquares();
    void lightSquares();
    void handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750);
    bool handleJoyPress(bool buttonState, uint8_t& gameState, uint8_t* targetSqrArr, short& targetSqrIdx);
    void reset() {score = 0; lives = 3; level = 1;}
};

// --------------------------------------------------------------
// ------------- Game Function Definitions ----------------------
// --------------------------------------------------------------
bool Game::matrix[MATRIX_SIZE][MATRIX_SIZE] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

LedSquare Game::squares[SQUARE_NUMBER/3][SQUARE_NUMBER/3] = {
  {LedSquare(0, 0), LedSquare(0, 3), LedSquare(0, 6)},
  {LedSquare(3, 0), LedSquare(3, 3), LedSquare(3, 6)},
  {LedSquare(6, 0), LedSquare(6, 3), LedSquare(6, 6)}
};

Game::Game() {
  this->score = 0;
  this->lives = 3;
  this->level = 1;
  this->currSqrRow = 1;
  this->currSqrCol = 1;
  this->currSqrState = true;
  for (uint8_t i = 0; i < SQUARE_NUMBER/3; i++) {
    for (uint8_t j = 0; j < SQUARE_NUMBER/3; j++) {
      squares[i][j].setState(matrix, true);
    }
  }
}

void Game::displayGame(LedControl& lc) {
  for (uint8_t i = 0; i < MATRIX_SIZE; i++) {
    for (uint8_t j = 0; j < MATRIX_SIZE; j++) {
      lc.setLed(0, i, j, matrix[i][j]);
    }
  }
}

void Game::wipeSquares() {
  for (uint8_t i = 0; i < SQUARE_NUMBER/3; i++) {
    for (uint8_t j = 0; j < SQUARE_NUMBER/3; j++) {
      squares[i][j].setState(matrix, false);
    }
  }
}

void Game::lightSquares() {
  for (uint8_t i = 0; i < SQUARE_NUMBER/3; i++) {
    for (uint8_t j = 0; j < SQUARE_NUMBER/3; j++) {
      squares[i][j].setState(matrix, true);
    }
  }
}

void Game::blinkCurrSquare(uint16_t blinkInterval) {
  if (squares[currSqrRow][currSqrCol].shouldBlink(blinkInterval) != currSqrState) {
    currSqrState = !currSqrState;
    squares[currSqrRow][currSqrCol].setState(matrix, currSqrState);
  }
}

void Game::handleJoyMove(uint16_t x, uint16_t y, bool& joyMoved, uint16_t minThresh=350, uint16_t maxThresh=750) {
  uint16_t xDefault = 460;
  uint16_t yDefault = 460;
  uint16_t errorMargin = 100;
  // x-axis input for changing current character position
  if (y >= yDefault - errorMargin && y <= yDefault + errorMargin) {
    if (!joyMoved && x < minThresh) {
        joyMoved = true;
        squares[currSqrRow][currSqrCol].setState(matrix, true);  // in case of being unselected while blinking
        this->currSqrRow--;
        if (this->currSqrRow < 0) {this->currSqrRow = SQUARE_NUMBER/3 - 1;}
        squares[currSqrRow][currSqrCol].setState(matrix, false);  // make it obvious the square has changed
    }
    if (!joyMoved && x > maxThresh) {
        joyMoved = true;
        squares[currSqrRow][currSqrCol].setState(matrix, true);  // in case of being unselected while blinking
        this->currSqrRow++;
        if (this->currSqrRow == SQUARE_NUMBER/3) {this->currSqrRow = 0;}
        squares[currSqrRow][currSqrCol].setState(matrix, false);  // make it obvious the square has changed
    }
    if (joyMoved && x >= minThresh && x <= maxThresh) {
        joyMoved = false;
    }
  }
  // y-axis input for changing current character value
  if (x >= xDefault - errorMargin && x <= xDefault + errorMargin) {
    if (!joyMoved && y < minThresh) {
        joyMoved = true;
        squares[currSqrRow][currSqrCol].setState(matrix, true);  // in case of being unselected while blinking
        this->currSqrCol++;
        if (this->currSqrCol == SQUARE_NUMBER/3) {this->currSqrCol = 0;}
        squares[currSqrRow][currSqrCol].setState(matrix, false);  // make it obvious the square has changed
    }
    if (!joyMoved && y > maxThresh) {
        joyMoved = true;
        squares[currSqrRow][currSqrCol].setState(matrix, true);  // in case of being unselected while blinking
        this->currSqrCol--;
        if (this->currSqrCol < 0) {this->currSqrCol = SQUARE_NUMBER/3 - 1;}
        squares[currSqrRow][currSqrCol].setState(matrix, false);  // make it obvious the square has changed
    }
    if (joyMoved && y >= minThresh && y <= maxThresh) {
        joyMoved = false;
    }
  }
}

bool Game::handleJoyPress(bool buttonState, uint8_t& gameState, uint8_t* targetSqrArr, short& targetSqrIdx) {
  // returns true if choice is correct, false otherwise - used to animate a correct choice
  if (targetSqrIdx == level) {  // all squares chosen correctly
    gameState = GAME_STATES::RANDOMIZE;
    targetSqrIdx = 0;
    level++;
    score++;  // for now score is equal to the number of levels passed - TODO: score by measuring completion time
    return false;
  }
  if (buttonState == LOW) {
    // get position of the target square
    short sqrRow = targetSqrArr[targetSqrIdx] / 3;
    short sqrCol = targetSqrArr[targetSqrIdx] % 3;
    if (sqrRow != currSqrRow || sqrCol != currSqrCol) {  // if the chosen square is wrong
      lives--;
      gameState = GAME_STATES::SHOW;  // show the user the correct configuration again
      this->wipeSquares();  // turn off all squares to prepare for the show state
      targetSqrIdx = 0;  // reset index to show all squares in the configuration
      return false;
    }
    else {  // chosen square is correct
      targetSqrIdx++;
      return true;
    }
  }
  return false;
}

#endif
