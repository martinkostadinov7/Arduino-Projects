#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Servo.h>

MPU6050 mpu;
int16_t ax, ay, az;
Servo servo;

float smoothedAngle = 0;
float peakAngle = 0;
bool movingRight = true;
unsigned long lastSwitchTime = 0;

// ПАРАМЕТРИ ЗА 60СМ МАХАЛО
const float filterCoeff = 0.05;      // Колкото по-малко, толкова по-инертен е сензорът
const float angleTolerance = 4.0;    // Градуси отклонение от пика, преди да обърне
const int lockoutTime = 700;         // Милисекунди забрана за повторна смяна (0.7 сек)

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  servo.attach(9);
  servo.write(90); // Старт в неутрално положение
  
  // Филтър на самия чип - помага за премахване на механичния шум от сервото
  mpu.setDLPFMode(6); // Най-силната хардуерна филтрация (5Hz)
}

void loop() {
  mpu.getAcceleration(&ax, &ay, &az);

  // Изчисляваме ъгъла по оста, която съвпада с движението на махалото
  // Ако махалото се движи напред-назад, ползваме Y и Z
  float currentAngle = atan2(ay, az) * 180 / M_PI;

  // Експоненциално изглаждане
  smoothedAngle = (smoothedAngle * (1.0 - filterCoeff)) + (currentAngle * filterCoeff);

  unsigned long currentTime = millis();

  // Логика за замах (Swing logic)
  if (currentTime - lastSwitchTime > lockoutTime) {
    
    // Ако се движим надясно и започнем да се връщаме
    if (movingRight && (smoothedAngle < peakAngle - angleTolerance)) {
      servo.write(0); // "Ритник" в едната посока
      movingRight = false;
      peakAngle = smoothedAngle;
      lastSwitchTime = currentTime;
      Serial.println("<<< MAX REACHED (RIGHT) - KICK LEFT");
    } 
    // Ако се движим наляво и започнем да се връщаме
    else if (!movingRight && (smoothedAngle > peakAngle + angleTolerance)) {
      servo.write(180); // "Ритник" в другата посока
      movingRight = true;
      peakAngle = smoothedAngle;
      lastSwitchTime = currentTime;
      Serial.println(">>> MAX REACHED (LEFT) - KICK RIGHT");
    }
  }

  // Обновяване на пика (най-крайната точка)
  if (movingRight && smoothedAngle > peakAngle) peakAngle = smoothedAngle;
  if (!movingRight && smoothedAngle < peakAngle) peakAngle = smoothedAngle;

  // Полезно за диагностика в Serial Plotter
  Serial.print(smoothedAngle);
  Serial.print(" ");
  Serial.println(peakAngle);

  delay(15);
}