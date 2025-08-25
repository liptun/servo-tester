#define UTILS_H

#include <Arduino.h>

String displayAnge(boolean active, String label, int angle);

int timeBasedStep(unsigned long deltaT);
