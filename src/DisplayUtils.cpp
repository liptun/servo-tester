#include <Arduino.h>

String displayAnge(boolean active, String label, int angle) {
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

  output += angleStr + "deg";
  return output;
}

