#include "Adafruit_SSD1306.h"
#include <Arduino.h>
#include <DisplayHelper.h>
#include <EncoderHelper.h>
#include <Servo.h>
#include <Utils.h>

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define STEPS_PER_CLICK 4

#define NEUTAR_PULSE 1500
#define NEUTAR_ANGLE 90

#define TEST_SERVO_PIN 9
Servo testServo;
byte angle = NEUTAR_ANGLE;
byte angleA = NEUTAR_ANGLE;
byte angleB = NEUTAR_ANGLE;
int pulse = NEUTAR_PULSE;
int pulseA = NEUTAR_PULSE;
int pulseB = NEUTAR_PULSE;
boolean isPrimaryAngleActive = true;
boolean isPulseMode = true;

DisplayHelper Display;

EncoderHelper Encoder(CLK_PIN, DT_PIN, SW_PIN, STEPS_PER_CLICK);

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Servo tester");

  testServo.attach(TEST_SERVO_PIN);
  testServo.write(0);

  Encoder.onTurn = [](int8_t direction, unsigned long deltaT) {
    Serial.println("Turn " + String(direction) + " " + String(deltaT));
    if (isPulseMode) {
      int newPulse = constrain(pulse + timeBasedStep(deltaT) * 10 * direction, 500, 2500);
      if (isPrimaryAngleActive) {
        pulseA = newPulse;
      } else {
        pulseB = newPulse;
      }
    } else {
      byte newAngle =
          constrain(angle + timeBasedStep(deltaT) * direction, 0, 180);
      if (isPrimaryAngleActive) {
        angleA = newAngle;
      } else {
        angleB = newAngle;
      }
    }
  };

  Encoder.onButtonClick = [](unsigned long pressDuration) {
    Serial.println("Click " + String(pressDuration));
    if (pressDuration < 1000) {
      isPrimaryAngleActive = !isPrimaryAngleActive;
    } else {
      isPulseMode = !isPulseMode;
    }
  };

  Display.init();

  Display.onRender = [](Adafruit_SSD1306 &display) {
    display.println();
    display.println(displayAnge(isPrimaryAngleActive, "A",
                                isPulseMode ? pulseA : angleA,
                                isPulseMode ? "ms" : "deg"));
    display.println(displayAnge(!isPrimaryAngleActive, "B",
                                isPulseMode ? pulseB : angleB,
                                isPulseMode ? "ms" : "deg"));
  };
}

void loop() {
  Encoder.update();
  Display.update();

  if (isPulseMode) {
    pulse = isPrimaryAngleActive ? pulseA : pulseB;
    testServo.writeMicroseconds(pulse);
  } else {
    angle = isPrimaryAngleActive ? angleA : angleB;
    testServo.write(angle);
  }
}
