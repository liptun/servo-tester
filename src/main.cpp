#include "Adafruit_SSD1306.h"
#include <Arduino.h>
#include <Config.h>
#include <DisplayHelper.h>
#include <EncoderHelper.h>
#include <Servo.h>
#include <Utils.h>

Servo testServo;
byte angle = SERVO_NEUTAR_ANGLE;
byte angleA = SERVO_NEUTAR_ANGLE;
byte angleB = SERVO_NEUTAR_ANGLE;
int pulse = SERVO_NEUTAR_PULSE;
int pulseA = SERVO_NEUTAR_PULSE;
int pulseB = SERVO_NEUTAR_PULSE;
boolean isPrimaryAngleActive = true;
boolean isPulseMode = true;

DisplayHelper Display;

EncoderHelper Encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_SW_PIN,
                      ENCODER_STEPS_PER_CLICK);

void setup() {
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  Serial.begin(BAUD_RATE);
  Serial.println("Servo tester");

  testServo.attach(TEST_SERVO_PIN);

  Encoder.onTurn = [](int8_t direction, unsigned long deltaT) {
    Serial.println("Turn " + String(direction) + " " + String(deltaT));
    if (isPulseMode) {
      int newPulse = constrain(pulse + timeBasedStep(deltaT) *
                                           MS_MODE_MULTIPLER * direction,
                               SERVO_MIN_US, SERVO_MAX_US);
      if (isPrimaryAngleActive) {
        pulseA = newPulse;
      } else {
        pulseB = newPulse;
      }
    } else {
      byte newAngle = constrain(angle + timeBasedStep(deltaT) * direction,
                                SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
      if (isPrimaryAngleActive) {
        angleA = newAngle;
      } else {
        angleB = newAngle;
      }
    }
  };

  Encoder.onButtonClick = [](unsigned long pressDuration) {
    Serial.println("Click " + String(pressDuration));
    if (pressDuration < BUTTON_LONG_PRESS_DURATION) {
      isPrimaryAngleActive = !isPrimaryAngleActive;
    } else {
      if (isPulseMode) {
        angleA = pulseToDeg(pulseA);
        angleB = pulseToDeg(pulseB);
      } else {
        pulseA = degToPulse(angleA);
        pulseB = degToPulse(angleB);
      }
      isPulseMode = !isPulseMode;
    }
  };

  Display.init();

  Display.onRender = [](Adafruit_SSD1306 &display) {
    display.println();
    display.println(displayServoPosition(
        isPrimaryAngleActive, "A", isPulseMode ? pulseA : angleA, isPulseMode));
    display.println(displayServoPosition(!isPrimaryAngleActive, "B",
                                         isPulseMode ? pulseB : angleB,
                                         isPulseMode));
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
