#ifndef GAME_H
#define GAME_H

class Game {
  private:
    int score;
    uint8_t lives;
    uint8_t level;
  public:
    Game();
    int getScore() {return score;}
    uint8_t getLives() {return lives;}
    uint8_t getLevel() {return level;}
};

Game::Game() {
  this->score = 0;
  this->lives = 3;
  this->level = 1;
}

#endif
