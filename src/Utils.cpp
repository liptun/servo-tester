#include <Arduino.h>

String displayAnge(boolean active, String label, int angle, String unit) {
  const byte angleStringTargetLen = 3;
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

  output += angleStr + unit;
  return output;
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
