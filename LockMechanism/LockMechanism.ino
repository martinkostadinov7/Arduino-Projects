#include <Servo.h>
#include <Keypad.h>

int redLed = 4;
int greenLed = 3;
int buzzer = 6;
int button = 5;

const byte ROWS = 4; 
const byte COLS = 3; 
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10}; 
byte colPins[COLS] = {9, 8, 7}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

bool locked = true;
char correctCombination[6] = {'0', '0', '0', '0', '0', '0'};
char combination[6] = {' ', ' ',' ', ' ',' ', ' '};
int index = 0;

int currentButton = 0;
int prevButton = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  myservo.write(90);
}

void loop() {
  if(!locked){
    currentButton = digitalRead(button);
    if(currentButton != prevButton && prevButton == 0){
      delay(100);
      lock();
    }
    prevButton = currentButton;
  }
  char customKey = customKeypad.getKey();
  
  if (customKey){
    if(!locked && customKey == '*'){
      digitalWrite(greenLed, 1);
      tone(buzzer, 622);
      delay(500);
      noTone(buzzer);
      setNewPassword();
      digitalWrite(greenLed, 1);
      tone(buzzer, 622);
      delay(100);
      tone(buzzer, 726);
      delay(500);
      noTone(buzzer);
      digitalWrite(greenLed, 0);
      delay(50);
    }
    else{
      combination[index] = customKey;
      tone(buzzer, random(500, 1000));
      delay(100);
      noTone(buzzer);
      index++;
    }
  }

  if(index >= 6) {
    if(checkCombination()){
      tone(buzzer, 1244);
      digitalWrite(greenLed, 1);
      delay(500);
      noTone(buzzer);
      digitalWrite(greenLed, 0);
      unlock();
    }
    else{
      tone(buzzer, 155);
      digitalWrite(redLed, 1);
      delay(500);
      noTone(buzzer);
      digitalWrite(redLed, 0);
    }

    resetCombination();
  }
}
bool checkCombination(){
  for(int i = 0; i < 6; i++){
    if(combination[i] != correctCombination[i]){
      return false;
    }
  }
  return true;
}

void resetCombination(){
   for(int i = 0; i < 6; i++){
    combination[i] = ' '; 
  }
  index = 0;
}

void unlock(){
  myservo.write(0);
  locked = false;
}

void lock(){
  myservo.write(90);
  tone(buzzer, 659);
  digitalWrite(greenLed, 1);
  digitalWrite(redLed, 1);
  delay(750);
  digitalWrite(greenLed, 0);
  digitalWrite(redLed, 0);
  noTone(buzzer);
  locked = true;
}

void setNewPassword(){
  int currentIndex = 0;
  while(currentIndex < 6){
    char customKey = customKeypad.getKey();
  
    if (customKey){
      correctCombination[currentIndex] = customKey;
      tone(buzzer, random(500, 1000));
      delay(100);
      noTone(buzzer);
      currentIndex++;
    }
  }
}