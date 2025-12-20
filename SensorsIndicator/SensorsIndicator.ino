#include <Servo.h>

int lightSensor = A5;
int piezoSensor = A0;
int tempSensor = A4;
int piezoLed = 9;
int tempLed = 8;
int lightLed = 2;
int button = 4;

Servo servo;  

void setup() {
  pinMode(lightSensor, INPUT);
  pinMode(lightLed, OUTPUT);
  pinMode(piezoSensor, INPUT);
  pinMode(piezoLed, OUTPUT);
  pinMode(tempSensor, INPUT);
  pinMode(tempLed, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  servo.attach(3);  
  servo.write(0);                  
}

int currentButton = 1;
int prevButton = 1;

int mode = 0; //0 - light 1 - temp 2- piezo

void loop() {
  updateMode();

  digitalWrite(lightLed, mode == 1);
  digitalWrite(tempLed,  mode == 2);
  digitalWrite(piezoLed, mode == 3);

  switch (mode) {
    case 0: servo.write(0); break;
    case 1: measureLight(); break;
    case 2: measureTemp();  break;
    case 3: measurePiezo(); break;
  }
}

void updateMode() {
  if (buttonPressed()) {
    mode = (mode + 1) % 4;
  }
}

bool buttonPressed() {
  currentButton = digitalRead(button); 
  if(prevButton != currentButton && prevButton == 1){
    prevButton = currentButton;
    return true;
  }
  prevButton = currentButton;
  return false;
  delay(50);
}


void measureLight(){
  int lightValue = analogRead(lightSensor);
  Serial.println(lightValue);
  int angle = map(lightValue, 0, 1023, 0, 180);     
  servo.write(angle);   
  delay(15); 
}


void measureTemp() {
  static long acc = 0;
  static int count = 0;

  acc += analogRead(tempSensor);
  count++;

  if (count >= 10000) {
    int avg = acc / count;
    Serial.print(avg);
    Serial.print(" ");
    float temperature = (((avg /1024.0) * 5.0) - 0.5) * 100.0;
    Serial.print(temperature);
    Serial.println(" C");
    int angle = map(avg, 140, 175, 0, 180);
    servo.write(angle);

    acc = 0;
    count = 0;
  }
}

void measurePiezo() {
  static int maxValue = 0;

  int val = analogRead(piezoSensor);
  if (val > maxValue) maxValue = val;

  static unsigned long lastReset = 0;
  if (millis() - lastReset > 500) {
    Serial.println(maxValue);
    servo.write(map(maxValue, 0, 1023, 0, 180));
    maxValue = 0;
    lastReset = millis();
  }
}
