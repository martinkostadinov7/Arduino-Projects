#include <Servo.h>

int leftSensor = A3;
int rightSensor = A2;
Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(11);
  servo.write(90);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  Serial.begin(9600);
}

// left
// light - 640-900
// covered - 200-470

// right
// light - 680-900
// covered - 250-430

int leftLightOnMax = 900; 
int leftLightOnMin = 830;
int leftCoveredMax = 770;
int leftCoveredMin = 500;
 

int rightLightOnMax = 900; 
int rightLightOnMin = 680;
int rightCoveredMax = 740;
int rightCoveredMin = 470;

int servoValue = 90;

bool leftCovered = false;
bool rightCovered = false;
int direction = 2; // 0 - <<<<<<<<<<     1 - >>>>>>>>>>     2- undefined 
void loop() {
  int leftValue = analogRead(leftSensor);
  int rightValue = analogRead(rightSensor);
  Serial.println(String(leftValue) + " " + String(rightValue));
  if(direction == 2){
    servo.write(90);
    if(leftValue > leftCoveredMin && leftValue < leftCoveredMax) {
      leftCovered = true;
      if(rightCovered){ // <<<<<<<<<<
        direction = 0;
      }    
    }  
    if(rightValue > rightCoveredMin && rightValue < rightCoveredMax) {
      rightCovered = true;
      if(leftCovered){ // >>>>>>>>>>
        direction = 1;
      }    
    }  
  }
  else{
    Serial.println("Found directiton");
    if(direction == 0){
      if(leftValue > leftCoveredMax){
        if(rightValue > rightCoveredMin && rightValue < rightCoveredMax){
          direction = 1;
        }
        servoValue-= 7;
      }
    }
    else if(direction == 1){
      if(rightValue > rightCoveredMax){
        if(leftValue > leftCoveredMin && leftValue < leftCoveredMax){
          direction = 0;
        }
        servoValue+= 7;
      }
    }
  }
  servo.write(servoValue);
  delay(1);
}
