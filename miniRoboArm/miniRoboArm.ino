#include <Servo.h>
int xJoy = A0;
int yJoy = A1;

Servo xServo;
Servo yServo;
void setup() {
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  xServo.attach(10);
  yServo.attach(9);

  Serial.begin(9600);
}
void loop() {
  int xVal = analogRead(xJoy);
  int yVal = analogRead(yJoy);
  
  int xServoVal;
  int yServoVal;
  if (xVal > 490 && xVal < 530) {
    xServo.write(90); 
  } else {
    xServoVal = map(xVal, 0, 1023, 70, 130);
    xServo.write(xServoVal);
  }
  
  if (yVal > 490 && yVal < 530) {
    yServo.write(90);
  } else {
    yServoVal = map(yVal, 0, 1023, 70, 130);
    yServo.write(yServoVal);
  }

  Serial.println("Servo X: " + String(xServoVal) + "   " + "Servo Y: " + String(yServoVal));
  delay(10);
}