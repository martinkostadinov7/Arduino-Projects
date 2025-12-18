// C++ code
//


int ledF = 13;
int ledD = 12;
int ledC = 11;
int ledB = 10;
int ledA = 9;
int ledG = 8;

int buzzer = 7;

#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 1, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup()
{
  pinMode(ledC, OUTPUT);
  pinMode(ledD, OUTPUT);
  pinMode(ledF, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("chestit rojden");
  lcd.setCursor(0, 1);
  lcd.print("den - 50!");
}

void loop()
{
  playHappyBirthday();
}

void playHappyBirthday() {

    // Нотни честоти
    const int G4 = 392;
    const int A4 = 440;
    const int B4 = 494;    
    const int C5 = 523;
    const int D5 = 587;
    const int E5 = 659;
    const int F5 = 698;
    const int G5 = 783;


    // За по-лесно писане
    auto play = [&](int led, int freq, int duration) {
        digitalWrite(led, 1);
        tone(buzzer, freq);
        delay(duration);
        digitalWrite(led, 0);
        noTone(buzzer);
        delay(50);
    };

    // Ред 1: G G A G C B
    play(ledG, G4, 200);
    play(ledG, G4, 200);
    play(ledA, A4, 400);
    play(ledG, G4, 400);
    play(ledC, C5, 400);
    play(ledB, B4, 800);

    // Ред 2: G G A G D C
    play(ledG, G4, 200);
    play(ledG, G4, 200);
    play(ledA, A4, 400);
    play(ledG, G4, 400);
    play(ledD, D5, 400);
    play(ledC, C5, 800);

    // Ред 3: G G G E C B A
    play(ledG, G4, 200);
    play(ledG, G4, 200);
    play(ledG, G5, 400);
    play(ledF, E5, 400);
    play(ledC, C5, 400);
    play(ledB, B4, 400);
    play(ledA, A4, 800);

    // Ред 4: F F E C D C
    play(ledF, F5, 200);
    play(ledF, F5, 200);
    play(ledD, E5, 400);
    play(ledB, C5, 400);
    play(ledC, D5, 400);
    play(ledB, C5, 1000);
}


