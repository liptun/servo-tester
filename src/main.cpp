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
boolean isSwingMode = false;
int swingDuration = 1000;
boolean isSwingForward = true;
unsigned long lastPulseMillis = 0;

DisplayHelper Display;

EncoderHelper Encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_SW_PIN, ENCODER_STEPS_PER_CLICK);

void setup() {
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  Serial.begin(BAUD_RATE);
  Serial.println("Servo tester");

  testServo.attach(TEST_SERVO_PIN);

  Encoder.onTurn = [](int8_t direction, unsigned long deltaT) {
    Serial.println("Turn " + String(direction) + " " + String(deltaT));
    if (isSwingMode) {
      swingDuration = constrain(swingDuration + timeBasedStep(deltaT) * SWING_MODE_MULTIPLER * direction, SWING_DURATION_MIN, SWING_DURATION_MAX);
    } else {
      if (isPulseMode) {
        int newPulse = constrain(pulse + timeBasedStep(deltaT) * MS_MODE_MULTIPLER * direction, SERVO_MIN_US, SERVO_MAX_US);
        if (isPrimaryAngleActive) {
          pulseA = newPulse;
        } else {
          pulseB = newPulse;
        }
        Serial.println("newPulse " + String(newPulse));
      } else {
        byte newAngle = constrain(angle + timeBasedStep(deltaT) * direction, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
        if (isPrimaryAngleActive) {
          angleA = newAngle;
        } else {
          angleB = newAngle;
        }
        Serial.println("newAngle " + String(newAngle));
      }
    }
  };

  Encoder.onButtonClick = [](unsigned long pressDuration) {
    Serial.println("__________________");
    Serial.println("Click " + String(pressDuration));
    if (pressDuration > BUTTON_LONG_PRESS_DURATION) {
      isSwingMode = !isSwingMode;
      Serial.print("Long press: ");
      Serial.println(isSwingMode ? "swing" : "normal");
      if (isSwingMode) {
        if (isPulseMode == false) {
          isPulseMode = true;
          pulseA = degToPulse(angleA);
          pulseB = degToPulse(angleB);
        }
      }
    } else if (pressDuration > BUTTON_SHORT_PRESS_DURATION) {
      if (isPulseMode) {
        angleA = pulseToDeg(pulseA);
        angleB = pulseToDeg(pulseB);
      } else {
        pulseA = degToPulse(angleA);
        pulseB = degToPulse(angleB);
      }
      isPulseMode = !isPulseMode;

      Serial.print("Short press: ");
      Serial.println(isPulseMode ? "pulse" : "deg");
    } else {
      Serial.print("Click: ");
      isPrimaryAngleActive = !isPrimaryAngleActive;
      Serial.println(isPrimaryAngleActive ? "primary" : "secondary");
    }
  };

  Display.init();

  Display.onRender = [](Adafruit_SSD1306 &display) {
    display.println(isSwingMode ? "Swing" : "Manual");
    if (isSwingMode) {

      display.println("Dur " + digit(swingDuration, 4) + "ms");

      display.print(isSwingForward ? " >>" : " <<");
      display.println(" " + digit(pulse, 4) + "ms");

    } else {
      display.println(displayServoPosition(isPrimaryAngleActive, "A", isPulseMode ? pulseA : angleA, isPulseMode));
      display.println(displayServoPosition(!isPrimaryAngleActive, "B", isPulseMode ? pulseB : angleB, isPulseMode));
    }
  };
}

void loop() {
  Encoder.update();
  Display.update();

  if (isSwingMode) {
    unsigned long currentPulseMillis = millis();
    if (currentPulseMillis - lastPulseMillis >= swingDuration) {
      lastPulseMillis = currentPulseMillis;
      isSwingForward = !isSwingForward;
    }

    float progress = float(currentPulseMillis - lastPulseMillis) / float(swingDuration);
    if (progress > 1.0)
      progress = 1.0;

    if (isSwingForward) {
      pulse = lerp(pulseA, pulseB, progress);
    } else {
      pulse = lerp(pulseB, pulseA, progress);
    }
    testServo.writeMicroseconds(pulse);

  } else {
    if (isPulseMode) {
      pulse = isPrimaryAngleActive ? pulseA : pulseB;
      testServo.writeMicroseconds(pulse);
    } else {
      angle = isPrimaryAngleActive ? angleA : angleB;
      testServo.write(angle);
    }
  }
}
