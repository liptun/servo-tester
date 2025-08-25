#define UTILS_H

#include <Arduino.h>

String displayAnge(boolean active, String label, int angle, String unit);

int timeBasedStep(unsigned long deltaT);
