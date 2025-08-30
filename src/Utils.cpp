#include <Arduino.h>
#include <Config.h>

String displayServoPosition(boolean active, String label, int angle, boolean isPulseMode) {
  const byte angleStringTargetLen = isPulseMode ? 4 : 3;
  String output = "";
  if (active) {
    output += "[";
    output += label;
    output += "] ";
  } else {
    output += " ";
    output += label;
    output += "  ";
  }

  String angleStr = String(angle);
  while (angleStr.length() < angleStringTargetLen)
    angleStr = " " + angleStr;

  String unit = isPulseMode ? "us" : "deg";
  output += angleStr + unit;
  return output;
}

String digit(int number, int digits) {
  String numberStr = String(number);
  while (numberStr.length() < digits)
    numberStr = " " + numberStr;
  return numberStr;
}

int timeBasedStep(unsigned long deltaT) {
  int step = 1;
  if (deltaT < 50)
    step = 6;
  else if (deltaT < 100)
    step = 3;
  else if (deltaT < 200)
    step = 2;
  else
    step = 1;
  return step;
}

int degToPulse(int angle) {
  angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  return map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_MIN_US, SERVO_MAX_US);
}

int pulseToDeg(int pulse) {
  pulse = constrain(pulse, SERVO_MIN_US, SERVO_MAX_US);
  return map(pulse, SERVO_MIN_US, SERVO_MAX_US, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
}

int lerp(int a, int b, float t) {
  return a + (int)((b - a) * t);
}
