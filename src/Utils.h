#define UTILS_H

#include <Arduino.h>

String displayServoPosition(boolean active, String label, int angle, boolean isPulseMode);

int timeBasedStep(unsigned long deltaT);

int degToPulse(int angle);
int pulseToDeg(int pulse);
