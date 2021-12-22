#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LedControl.h>
#include "SetupConstants.h"
#include "Menu.h"
#include "Game.h"

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);
LcdMenu* currentMenu;
bool drawMenu = false;
unsigned long long lastMenuChange;
uint16_t menuChangeDelay = 750;

bool joyMoved = false;

LedControl ledControl = LedControl(pinDIN, pinCLK, pinCS, 1);
Game game;
bool inGame = false;
uint8_t gameState = GAME_STATES::RANDOMIZE;
uint8_t targetSqrArr[30];
short targetSqrIdx = 0;
bool targetBlinked = false;
unsigned long long targetLastBlinked = 0;
const uint16_t animationDuration = 500;

void setup() {
  Serial.begin(9600);
  
  readControlVariables();
  pinMode(pinContrast, OUTPUT);
  pinMode(pinBrightness, OUTPUT);
  analogWrite(pinContrast, lcdContrast.value);
  analogWrite(pinBrightness, lcdBrightness.value.toInt());

  pinMode(pinJoyVx, INPUT);
  pinMode(pinJoyVy, INPUT);
  pinMode(pinJoyBttn, INPUT_PULLUP);

  pinMode(pinBuzzer, OUTPUT);

  lcd.createChar(arrowNum, arrow);
  lcd.begin(lcdColNum, lcdRowNum);
  LcdNav::initArrow(arrowNum);

  initMenus();
  currentMenu = menuMain;
  currentMenu->display(lcd);

  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, ledBrightness.value.toInt());

  randomSeed(analogRead(0));

  showIntroScreen();
}

void loop() {
  handleLcdMenu();
  handleGame();
}

void showIntroScreen() {
  LcdText title("Human Benchmark");
  LcdText author("Brihac Andrei");
  title.display(lcd, title.getCol(), 0);
  author.display(lcd, author.getCol(), 1);
  while (digitalRead(pinJoyBttn) == HIGH) {}
  lastMenuChange = millis();
  drawMenu = true;
}

void handleLcdMenu() {
  // returns 'moved' only when the joyMoved variable transitions from false to true --> this is a signal to draw the modified menu
  uint16_t vxRead = analogRead(pinJoyVx);
  uint16_t vyRead = analogRead(pinJoyVy);
  String joyState = currentMenu->handleJoyMove(vyRead, vxRead, joyMoved);  // inversed reads because of the way I'm holding the joystick
  if (joyState == "moved") {
      drawMenu = true;
  }
  // returns null if button is not pressed or the current menu button doesn't do anything
  // returns a pointer to the chosen menu when the button is pressed
  // so when the pointer is not null it's a signal that a new menu must be drawn
  bool buttonState = digitalRead(pinJoyBttn);
  if (millis() - lastMenuChange > menuChangeDelay) {  // look for button input a short delay after changing menus
    LcdMenu* newMenu = currentMenu->handleJoyPress(buttonState);
    if (newMenu) {
        currentMenu = newMenu;
        drawMenu = true;
        lastMenuChange = millis();
    }
  }
  switch (currentMenu->getType()) {
    case MENU_TYPES::NAV:
    {
      lcd.noBlink();  // make sure the cursor isn't blinking after exiting an input menu
      break;
    }
    case MENU_TYPES::IN:
    {
      ((LcdInput*)currentMenu)->blinkCursor(lcd);  // blink the cursor at the chosen location
      break;
    }
    case MENU_TYPES::GAME:
    {
      inGame = true;
      drawMenu = ((LcdGame*)currentMenu)->setVariables(game.getScore(), game.getLives(), game.getLevel());  // this method returns true when any of the variables change value
      break;
    }
  }
  // if a signal to draw the menu has been received from moving or pressing the joystick, then we draw it and signal that it has been drawn
  if (drawMenu) {
    currentMenu->display(lcd);
    analogWrite(pinContrast, lcdContrast.value);
    analogWrite(pinBrightness, lcdBrightness.value.toInt());
    ledControl.setIntensity(0, ledBrightness.value.toInt());
    drawMenu = false;
  }
}

void handleGame() {
  if (!inGame) {return;}
  switch (gameState) {
    case GAME_STATES::SELECTION:
    {
      // get joystick reads
      uint16_t vxRead = analogRead(pinJoyVx);
      uint16_t vyRead = analogRead(pinJoyVy);
      bool buttonState = digitalRead(pinJoyBttn);
      // modifies the currSqrRow and currSqrCol variables in the Game class
      game.handleJoyMove(vxRead, vyRead, joyMoved);

      // checks if current square is the target square for each targetSqr in targetSqrArr
      // if chosen square is wrong moves to SHOW (to show the correct configuration) and decrements Game.lives 
      // if all choices are correct advances the level and moves to RANDOMIZE
      bool animateChoice = game.handleJoyPress(buttonState, gameState, targetSqrArr, targetSqrIdx);  // returns true if correct choice, false otherwise
      if (animateChoice) {  // animate a correct choice
        short sqrRow = targetSqrArr[targetSqrIdx - 1] / 3;
        short sqrCol = targetSqrArr[targetSqrIdx - 1] % 3;
        // turn off all squares and turn on the correct choice
        game.wipeSquares();
        Game::squares[sqrRow][sqrCol].setState(Game::matrix, true);
        game.displayGame(ledControl);
        delay(animationDuration);  // also gets rid of having to check for repeat presses of the button which is pretty cool - thanks delay!
        game.lightSquares();
      }
      
      game.blinkCurrSquare(selectionBlinkInterval);
      break;
    }
    case GAME_STATES::SHOW:
    {
      if (targetSqrIdx < game.getLevel()) {
        short sqrRow = targetSqrArr[targetSqrIdx] / 3;
        short sqrCol = targetSqrArr[targetSqrIdx] % 3;
        if (!targetBlinked) {  // we only blink each target square once, so this method is needed
          targetBlinked = true;
          targetLastBlinked = millis();
          tone(pinBuzzer, tones[sqrRow][sqrCol], buzzDuration);
          game.wipeSquares();  // turn squares off
          Game::squares[sqrRow][sqrCol].setState(Game::matrix, true);  // turn the target square on
        } else if (millis() - targetLastBlinked > showBlinkInterval) {
          targetBlinked = false;
          Game::squares[sqrRow][sqrCol].setState(Game::matrix, false);  // turn the target square off
          targetSqrIdx++;  // move on to the next target square
        }
      }
      else {
        game.lightSquares();  // turn all the squares back on to prepare for the selection state
        targetSqrIdx = 0;
        gameState = GAME_STATES::SELECTION;
        game.setCurrSquare(1, 1); // set current square in the middle
      }
      break;
   }
   case GAME_STATES::RANDOMIZE:
   {
    targetSqrArr[game.getLevel() - 1] = random(SQUARE_NUMBER);  // add a random square index (0-8) to targetSqrArr (each time level advances one new square is added)
    gameState = GAME_STATES::SHOW;
    game.wipeSquares();  // turn off all squares and only turn on the target square, making it easier to see
    break;
   }
  }
  
  if (game.getLives() == 0) {
    // the user ran out of lives, reset the game for next player and wipe the display
    checkHighScore();
    inGame = false;
    game.reset();
    game.wipeSquares();
    gameState = GAME_STATES::RANDOMIZE;
    // send the user to a score screen
    currentMenu = menuEnd;
    drawMenu = true;
    lastMenuChange = millis();
  }
  
  game.displayGame(ledControl);
}

void checkHighScore() {
  String endScore;
  if (game.getScore() > highScore.value) {
    highScore.setVal(game.getScore());
    highScore.saveVal();
    highScoreUserName.setVal(userName.value);
    highScoreUserName.saveVal();
    endScore = String("NewHighScore:");
    ((LcdNav*)menuScore)->setBttnText(highScoreUserName.value + String(':') + String(highScore.value), 0);
  }
  else {endScore = userName.value + String(':');}
  endScore += String(game.getScore());
  menuEnd->setTitle(endScore);
}
