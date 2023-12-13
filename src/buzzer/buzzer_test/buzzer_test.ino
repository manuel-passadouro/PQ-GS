#include "pitches.h"
#include "xmas-songs.h"

// BUZZER WORKING 8/12/2023

#define BUZZERPIN   PC_4

void setup() {
  pinMode(BUZZERPIN, OUTPUT);
}

void loop() {
  weWishYou(160);    // songTempo is a uint8_t so it can be 0-255.
  delay(2000);

  //All the songs repository with the respectively velocities

  //awayInAManger(125); 
  //carolOfTheBells(135);
  //deckTheHalls(150);
  //goTellItOnTheMountain(140);
  //godRestYe(170);
  //harkTheHerald(140);
  //jingleBells(180);
  //joyToTheWorld(160);
  //oChristmasTree(115);
  //oComeAllYe(140);
  //oLittleTown(125);
  //rudolfTheRedNosed(150);
  //santaClausIsComin(137);
  //silentNight(130);
  //theFirstNoel(130);
  //weThreeKings(144);
  //weWishYou(160);
  //whiteChristmas(155);
}
