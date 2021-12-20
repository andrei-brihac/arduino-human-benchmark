#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 8
#define SQUARE_NUMBER 9

#include <LedControl.h>

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
};

class Game {
  private:
    int score;
    uint8_t lives;
    uint8_t level;
  public:
    static bool matrix[MATRIX_SIZE][MATRIX_SIZE];
    static LedSquare squares[SQUARE_NUMBER];
    Game();
    int getScore() {return score;}
    uint8_t getLives() {return lives;}
    uint8_t getLevel() {return level;}
    void displayGame(LedControl& lc);
};

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

LedSquare Game::squares[SQUARE_NUMBER] = {
  LedSquare(0, 0), LedSquare(0, 3), LedSquare(0, 6),
  LedSquare(3, 0), LedSquare(3, 3), LedSquare(3, 6),
  LedSquare(6, 0), LedSquare(6, 3), LedSquare(6, 6)
};

Game::Game() {
  this->score = 0;
  this->lives = 3;
  this->level = 1;
  for (uint8_t i = 0; i < SQUARE_NUMBER; i++) {
    squares[i].setState(matrix, true);
  }
}

void Game::displayGame(LedControl& lc) {
  for (uint8_t i = 0; i < MATRIX_SIZE; i++) {
    for (uint8_t j = 0; j < MATRIX_SIZE; j++) {
      lc.setLed(0, i, j, matrix[i][j]);
    }
  }
}

#endif
