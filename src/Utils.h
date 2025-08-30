#define UTILS_H

#include <Arduino.h>

String displayServoPosition(boolean active, String label, int angle, boolean isPulseMode);

String digit(int number, int digits);

int timeBasedStep(unsigned long deltaT);

int degToPulse(int angle);
int pulseToDeg(int pulse);

int lerp(int a, int b, float t);
