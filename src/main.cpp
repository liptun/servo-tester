#include "Adafruit_SSD1306.h"
#include <Arduino.h>
#include <DisplayHelper.h>
#include <Utils.h>
#include <EncoderHelper.h>
#include <Servo.h>

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define STEPS_PER_CLICK 4

#define TEST_SERVO_PIN 9
Servo testServo;
byte angle = 0;
byte angleA = 0;
byte angleB = 0;
boolean isPrimaryAngleActive = true;

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
    byte newAngle =
        constrain(angle + timeBasedStep(deltaT) * direction, 0, 180);
    if (isPrimaryAngleActive) {
      angleA = newAngle;
    } else {
      angleB = newAngle;
    }
  };

  Encoder.onButtonClick = []() {
    Serial.println("Click");
    isPrimaryAngleActive = !isPrimaryAngleActive;
  };

  Display.init();

  Display.onRender = [](Adafruit_SSD1306 &display) {
    display.println();
    display.println(displayAnge(isPrimaryAngleActive, "A", angleA));
    display.println(displayAnge(!isPrimaryAngleActive, "B", angleB));
  };
}

void loop() {
  Encoder.update();
  Display.update();

  angle = isPrimaryAngleActive ? angleA : angleB;
  testServo.write(angle);
}
