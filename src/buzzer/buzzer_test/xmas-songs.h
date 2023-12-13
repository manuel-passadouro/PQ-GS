#define BUZZERPIN   PC_4

void jingleBells(uint8_t songTempo)
{
  int16_t melody[] {
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_G5,4, NOTE_C5,4, NOTE_D5,4,
    NOTE_E5,-2, REST,8,
    NOTE_F5,4, NOTE_F5,4, NOTE_F5,-4, NOTE_F5,8,
    NOTE_F5,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,8, NOTE_E5,8,
    NOTE_E5,4, NOTE_D5,4, NOTE_D5,4, NOTE_E5,4,
    NOTE_D5,2, NOTE_G5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_G5,4, NOTE_C5,4, NOTE_D5,4,
    NOTE_E5,-2, REST,8,
    NOTE_F5,4, NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
    NOTE_F5,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,8, NOTE_E5,8,
    NOTE_G5,4, NOTE_G5,4, NOTE_F5,4, NOTE_D5,4,
    NOTE_C5,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void silentNight(uint8_t songTempo) 
{
  int16_t melody[] {
    NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
    NOTE_E4,-2, 
    NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
    NOTE_E4,-2, 
    NOTE_D5,2, NOTE_D5,4,
    NOTE_B4,-2,
    NOTE_C5,2, NOTE_C5,4,
    NOTE_G4,-2,
    NOTE_A4,2, NOTE_A4,4,
    NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
    NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
    NOTE_E4,-2, 
    NOTE_A4,2, NOTE_A4,4,
    NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
    NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
    NOTE_E4,-2, 
    NOTE_D5,2, NOTE_D5,4,
    NOTE_F5,-4, NOTE_D5,8, NOTE_B4,4,
    NOTE_C5,-2,
    NOTE_E5,-2,
    NOTE_C5,4, NOTE_G4,4, NOTE_E4,4,
    NOTE_G4,-4, NOTE_F4,8, NOTE_D4,4,
    NOTE_C4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void awayInAManger(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2,
    NOTE_G3,4, NOTE_G3,-4, NOTE_A3,8, NOTE_G3,4, NOTE_G3,4, NOTE_D4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_C4,4, NOTE_E4,2,
    NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2,
    NOTE_G3,4, NOTE_F4,-4, NOTE_E4,8, NOTE_D4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void carolOfTheBells(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4,
    NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_D5,4, NOTE_C5,8, NOTE_D5,8, NOTE_AS4,4, NOTE_G5,4, NOTE_G5,8, NOTE_G5,8, NOTE_F5,8, NOTE_DS5,8, NOTE_D5,4, NOTE_D5,8, NOTE_D5,8, NOTE_C5,8, NOTE_AS4,8,
    NOTE_C5,4, NOTE_C5,8, NOTE_C5,8, NOTE_D5,8, NOTE_C5,8, NOTE_AS4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_G4,4, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,8, NOTE_AS4,8, NOTE_C5,8, NOTE_D5,8, NOTE_C5,4, NOTE_AS4,4, NOTE_D5,8, NOTE_D5,8, NOTE_FS5,8, NOTE_G5,8, NOTE_A5,8, NOTE_AS5,8,
    NOTE_C6,8, NOTE_D6,8, NOTE_C6,4, NOTE_AS5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4, NOTE_AS5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,4,
    NOTE_AS5,-4, NOTE_A5,-8, NOTE_AS5,-8, NOTE_G5,2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void deckTheHalls(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_D4,8, NOTE_E4,-4, NOTE_D4,8, NOTE_C4,4, NOTE_B3,4, NOTE_C4,2, 
    NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_D4,8, NOTE_E4,-4, NOTE_D4,8, NOTE_C4,4, NOTE_B3,4, NOTE_C4,2,
    NOTE_D4,-4, NOTE_E4,8, NOTE_F4,4, NOTE_D4,4, NOTE_E4,-4, NOTE_F4,8, NOTE_G4,4, NOTE_D4,4, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,4, NOTE_A3,8, NOTE_B3,8, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,2, 
    NOTE_G4,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,8, NOTE_A3,8, NOTE_A3,8, NOTE_A3,8, NOTE_G3,-4, NOTE_F4,8, NOTE_E4,4, NOTE_D4,4, NOTE_C4,2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void goTellItOnTheMountain(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,2, NOTE_D4,8, NOTE_D4,4, NOTE_D4,8, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,4,
    NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,4, NOTE_F4,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,8, NOTE_C4,8, NOTE_D4,4, NOTE_C4,-2, NOTE_C4,4,
    NOTE_E4,4, NOTE_A4,4, NOTE_A4,-4, NOTE_A4,8, NOTE_A4,4, NOTE_E4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,4, NOTE_D4,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_C4,4,
    NOTE_E4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4, NOTE_E4,4, NOTE_E4,4, NOTE_C4,4, NOTE_D4,4, NOTE_D4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,2, NOTE_F4,2,
    NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,2, NOTE_D4,8, NOTE_D4,4, NOTE_D4,8, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_C4,4, NOTE_A3,4, NOTE_G3,4,
    NOTE_E4,2, NOTE_E4,8, NOTE_D4,8, NOTE_C4,8, NOTE_A3,8, NOTE_G3,2, NOTE_C4,4, NOTE_F4,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,8, NOTE_C4,8, NOTE_D4,4, NOTE_C4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void godRestYe(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_A3,4, NOTE_A3,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_A3,4,
    NOTE_A3,4, NOTE_E4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,4, NOTE_G3,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,4, NOTE_E4,-2, NOTE_E4,4, NOTE_G4,4, NOTE_D4,4, NOTE_E4,4, NOTE_G4,4,
    NOTE_G4,4, NOTE_A4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_A3,4, NOTE_B3,4, NOTE_C4,4, NOTE_D4,2, NOTE_C4,4, NOTE_D4,4, NOTE_E4,2, NOTE_G4,4, NOTE_E4,4,
    NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,2, NOTE_C4,8, NOTE_B3,8, NOTE_A3,4, NOTE_D4,2, NOTE_C4,4, NOTE_D4,4, NOTE_E4,4, NOTE_F4,4, NOTE_G4,4, NOTE_A4,4, NOTE_E4,4, NOTE_D4,4, NOTE_C4,4, NOTE_B3,4, NOTE_A3,1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void harkTheHerald(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_D4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_G4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_D5,-4, NOTE_C5,8, NOTE_B4,4, NOTE_A4,4, NOTE_B4,2, NOTE_D4,4, NOTE_G4,4, NOTE_G4,-4, NOTE_F4,8, NOTE_G4,4,
    NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_A4,4, NOTE_A4,-4, NOTE_F4,8, NOTE_F4,4, NOTE_E4,8, NOTE_D4,8, NOTE_D4,2, 

    NOTE_D5,4, NOTE_D5,4, NOTE_D5,4, NOTE_G4,4, NOTE_C5,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_D5,4, NOTE_G4,4, NOTE_C5,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_D5,4,
    NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,-4, NOTE_G4,8, NOTE_G4,4, NOTE_A4,4, NOTE_B4,2, 

    NOTE_E5,-4, NOTE_E5,8, NOTE_E5,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,-4, NOTE_G4,8, NOTE_G4,4, NOTE_A4,4, NOTE_G4,2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void joyToTheWorld(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_F5,2, NOTE_E5,-4, NOTE_D5,8, NOTE_C5,-2, NOTE_AS4,4, NOTE_A4,2, NOTE_G4,2, NOTE_F4,-2,
    NOTE_C5,4, NOTE_D5,-2, NOTE_D5,4, NOTE_E5,-2, NOTE_E5,4, NOTE_F5,1,
    NOTE_F5,4, NOTE_F5,4, NOTE_E5,4, NOTE_D5,4, NOTE_C5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_F5,4, NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
    NOTE_C5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_A4,8, NOTE_AS4,8, NOTE_C5,-2,
    NOTE_AS4,8, NOTE_A4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4, NOTE_G4,8, NOTE_A4,8, NOTE_AS4,-2,
    NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F5,2, NOTE_D5,4, NOTE_C5,-4, NOTE_AS4,8, NOTE_A4,4, NOTE_C5,4, NOTE_E4,2, NOTE_D4,2, NOTE_C4,1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void oChristmasTree(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_D4,4, NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,4, NOTE_D4,2, REST,16,
    NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,-4, REST,4, NOTE_D5,8,
    NOTE_D5,8, NOTE_B4,8, NOTE_E5,-4, NOTE_D5,8, NOTE_D5,8, NOTE_C5,8, NOTE_C5,-4, NOTE_C5,8, NOTE_C5,8, NOTE_A4,8, NOTE_D5,-4, NOTE_C5,8, NOTE_C5,8, NOTE_B4,8, NOTE_B4,4, NOTE_D4,4,
    NOTE_G4,-8, NOTE_G4,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-8, NOTE_B4,16, NOTE_B4,-4, NOTE_B4,8, NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_F4,4, NOTE_A4,4, NOTE_G4,2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void oComeAllYe(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_G4,4, NOTE_G4,2, NOTE_D4,4, NOTE_G4,4, NOTE_A4,2, NOTE_D4,2, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_C5,4, NOTE_B4,2,
    NOTE_A4,4, NOTE_G4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,4, NOTE_FS4,2, NOTE_E4,-4, NOTE_D4,8, NOTE_D4,1,
    NOTE_D5,2, NOTE_C5,4, NOTE_B4,4, NOTE_C5,2, NOTE_B4,2, NOTE_A4,4, NOTE_B4,4, NOTE_G4,4, NOTE_A4,4, NOTE_FS4,-4, NOTE_E4,8, NOTE_D4,4,
    NOTE_G4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_G4,2, NOTE_D4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_C5,4, NOTE_B4,2, NOTE_A4,4,
    NOTE_B4,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,2, NOTE_G4,4, NOTE_C5,4, NOTE_B4,2, NOTE_A4,-4, NOTE_G4,8, NOTE_G4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void oLittleTown(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4, NOTE_D4,4, NOTE_B4,-2,
    NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_E5,4, NOTE_D5,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2,
    NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,2, NOTE_FS4,4, NOTE_FS4,4, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,-2,
    NOTE_B4,4, NOTE_B4,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_E4,4, NOTE_E5,4, NOTE_D5,4, NOTE_G4,4, NOTE_B4,-4, NOTE_A4,8, NOTE_G4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void rudolfTheRedNosed(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_A4,4, NOTE_E4,1,
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1,
    NOTE_A4,4, NOTE_A4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,4, NOTE_E4,4, NOTE_G4,2, NOTE_F4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,4, NOTE_E4,1,
    NOTE_D4,4, NOTE_E4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,4, NOTE_B4,4, NOTE_B4,2, NOTE_C5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,8, NOTE_D4,-2,
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void santaClausIsComin(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_G4,8,
    NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
    NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
    NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
    NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
    NOTE_D4,4, NOTE_F4,2, NOTE_B3,4,
    NOTE_C4,-2, REST,4,
    NOTE_G4,8,
    NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
    NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
    NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
    NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
    NOTE_D4,4, NOTE_F4,2, NOTE_D5,4,
    NOTE_C5,1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void theFirstNoel(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4,
    NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4,
    NOTE_FS4,2, NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4,
    NOTE_A4,2, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4,
    NOTE_FS4,2, NOTE_FS4,8, NOTE_E4,8, NOTE_D4,-4, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2, NOTE_D5,8, NOTE_CS5,8, NOTE_B4,2, NOTE_B4,4,
    NOTE_A4,-2, NOTE_D5,4, NOTE_CS5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_CS5,4, NOTE_D5,4, NOTE_A4,4, NOTE_G4,4, NOTE_FS4,-2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void weThreeKings(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,-2, REST,4,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,2, NOTE_FS4,4, NOTE_E4,1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void weWishYou(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_C5,4,
    NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
    NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
    NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
    NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
    NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  
    NOTE_F5,2, NOTE_C5,4,
    NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
    NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
    NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
    NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
    NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
    NOTE_F5,2
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}

void whiteChristmas(uint8_t songTempo) {
  int16_t melody[] {
    NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, NOTE_D5,-2, REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4, NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
    NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2, NOTE_D5,4, NOTE_G4,2, NOTE_G4,2, NOTE_G4,4, NOTE_D5,2, NOTE_C5,4, NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_A4,-1,
    NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_AS4,4, NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, NOTE_D5,-2, REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4, NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
    NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2, NOTE_D5,4, NOTE_G5,1, REST,16, NOTE_G4,4, NOTE_A4,4, NOTE_B4,2, NOTE_B4,2, NOTE_E5,4, NOTE_E5,4, NOTE_FS4,4, NOTE_FS4,4, NOTE_G4,-1
  };
  uint16_t notes = sizeof(melody) / sizeof(melody[0]) / 2;
  uint16_t wholenote = (60000 * 4) / songTempo;
  int8_t divider;
  uint16_t noteDuration;
  for (uint16_t i=0; i<notes*2; i+=2) {
    divider = melody[i+1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = wholenote / abs(divider) * 1.5;
    }
    tone(BUZZERPIN, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZERPIN);
  }
}