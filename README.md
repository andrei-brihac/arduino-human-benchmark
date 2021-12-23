# arduino-human-benchmark

## Introduction

An Arduino sequence memory game, an implementation of [Human Benchmark - Sequence](https://humanbenchmark.com/tests/sequence).

Initially, I wanted to implement all the games from the **Human Benchmark** site, but I stumbled into some memory limitations which forced me to compromise on design choices and code architecture.

It turns out that trying to do a menu design based on using runtime polymorphism quickly runs out of memory on a 2 megabyte Arduino Uno. Still, I managed to code something half-way decent by my standards.

## Description

The game boils down to remembering the order in which squares light up in a 3x3 board. Each time a correct sequence is inputted by the player, the sequence length increases by one.

Check out the web-based game [here](https://humanbenchmark.com/tests/sequence).

At the start of every level, the target sequence is shown to the player along with sound cues for each square.

Then, the whole 3x3 board lights up and the player can select squares using the joystick.

When the player presses the joystick button the chosen square is checked against the target square in the sequence.
* Wrong choices make a distinct sound and deduct one life. Also, the target configuration is again shown to the player.
* Correct choices play the sound associated with the square and do a little "animation".

If the user makes all the correct choices, the game moves to the next level, increasing the sequence length by one. Score is equal to the number of levels passed.

Watch a full demo of the project on [my YouTube page](https://youtu.be/H0N2kV3h8FQ). Here's a picture of the setup.

<img src = "https://github.com/andrei-brihac/arduino-human-benchmark/blob/11a143ab7dec5b19c6653825cace9f4e247a8bbe/setup-image.png"/>

## Components Used

Here's a list of the components I used to build this project:
* a 16x2 LCD for the menu
* a MAX7219 display driver for controlling the LED matrix
* a 1088AS LED matrix for the game
* an electrolytic capacitor of 10 microfarads and a ceramic capacitor of 104 picofarads for filtering circuit noise
* an active buzzer for the sound
* a joystick for the player input
* wires and resistors per logic

## Progress Updates

Progress update 1 - [the menu](https://youtu.be/9R57YIZmz7A)

Progress update 2 - [the game](https://youtu.be/R_OLAYvbGoA)

Progress update 3 - [finishing touches](https://youtu.be/H0N2kV3h8FQ)
