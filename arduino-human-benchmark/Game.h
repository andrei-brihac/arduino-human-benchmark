#ifndef GAME_H
#define GAME_H

const uint8_t pinDIN = 12;
const uint8_t pinCS = 13;
const uint8_t pinCLK = 11;

uint8_t xPos = 4;
uint8_t yPos = 4;
uint8_t xLastPos = 4;
uint8_t yLastPos = 4;
bool matrixChanged = true;

const uint16_t minThresh = 200;
const uint16_t maxThresh = 600;

const uint8_t matrixSize = 8;
bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

unsigned long long lastMoved = 0;
const uint8_t moveInterval = 100;

int score = 0;
uint8_t lives = 3;
uint8_t level = 1;

class Food {
  public:
    uint8_t xPos;
    uint8_t yPos;
    Food(uint8_t xPos=0, uint8_t yPos=0) {
      this->xPos = xPos;
      this->yPos = yPos;
    }
    void placeFood(bool matrix[8][8], uint8_t matrixSize=8) {
      this->xPos = random(1, matrixSize - 1);
      this->yPos = random(1, matrixSize - 1);
      matrix[xPos][yPos] = 1;
    }
    void removeFood() {
      this->xPos = 0;
      this->yPos = 0;
    }
};

Food food;
unsigned long long lastFood = 0;
const uint16_t foodInterval = 500;
bool foodExists = false;

void initMatrixWalls() {
  for (uint8_t i = 0; i < matrixSize; i++) {
    matrix[i][0] = 1;
    matrix[i][matrixSize - 1] = 1;
    matrix[0][i] = 1;
    matrix[matrixSize - 1][i] = 1;
  }
}

void updateMatrix(LedControl& lc) {
  for (int row = 0; row < matrixSize; row++)
    for (int col = 0; col < matrixSize; col++)
      lc.setLed(0, row, col, matrix[row][col]);
}

void updatePositions() {
  int xValue = analogRead(pinJoyVx);
  int yValue = analogRead(pinJoyVy);

  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < minThresh) {
    if (xPos > 0) {
      xPos--;
    } 
    else {
      xPos = matrixSize - 1;
    }
  }

  if (xValue > maxThresh) {
    if (xPos < matrixSize - 1) {
      xPos++;
    }
    else {
      xPos = 0;
    }
  }

  if (yValue > maxThresh) {
    if (yPos > 0) {
      yPos--;
    } 
    else {
      yPos = matrixSize - 1;
    }
  }

  if (yValue < minThresh) {
    if (yPos < matrixSize - 1) {
      yPos++;
    }
    else {
      yPos = 0;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    matrix[xPos][yPos] = 1;
    matrix[xLastPos][yLastPos] = 0;
  }
}
bool hitWall(int x, int y) {
  return x == 0 || x == matrixSize - 1 || y == 0 || y == matrixSize - 1; 
}

void testGame(LedControl& lc) {
  if (millis() - lastMoved > moveInterval) {
    // game logic
    updatePositions();
    lastMoved = millis();
  }
  if (!foodExists && millis() - lastFood > foodInterval) {
    food.placeFood(matrix);
    foodExists = true;
  }
  if (xPos == food.xPos && yPos == food.yPos) {
    foodExists = false;
    score++;
    food.removeFood();
    lastFood = millis();
  }
  if (hitWall(xPos, yPos)) {
    lives--;
    xPos = 4;
    yPos = 4;
    matrix[xPos][yPos] = 1;
    matrixChanged = true;
  }
  // matrix display logic
  if (matrixChanged) {
    matrixChanged = false;
    updateMatrix(lc);
  }
}

#endif
