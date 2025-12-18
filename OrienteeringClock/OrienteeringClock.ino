#include "RTClib.h"

RTC_DS3231 rtc;
int led = 8;
int buzzer = 13;

int note1 = 659;
int note2 = 1318;

unsigned long prevSecondMillis = 0;
unsigned long beepStartMillis = 0;

int beepDuration = 0;
bool beeping = false;

void setup() {
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop () {

  // Таймер за 1 секунда
  if (millis() - prevSecondMillis >= 1000) {
    prevSecondMillis = millis();

    DateTime now = rtc.now();

    String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour());
    String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute());
    String secondStr = (now.second() < 10 ? "0" : "") + String(now.second());

    String formattedTime = hourStr + ":" + minuteStr + ":" + secondStr;

    switch (now.second()) {
      case 57: beep(note1, 500); break;
      case 58: beep(note1, 500); break;
      case 59: beep(note1, 500); break;
      case 0:  beep(note2, 1000); break;
    }

    Serial.println(formattedTime);
    Serial.println();
  }

  updateBeep();
}

void updateBeep() {
  if (beeping) {
    if (millis() - beepStartMillis >= beepDuration) {
      noTone(buzzer);
      digitalWrite(led, LOW);
      beeping = false;
    }
  }
}

void beep(int note, int duration) {
  tone(buzzer, note);
  digitalWrite(led, HIGH);

  beeping = true;
  beepDuration = duration;
  beepStartMillis = millis(); 
}
