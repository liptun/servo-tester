#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DisplayUtils.h>
#include <EncoderHelper.h>
#include <Servo.h>
#include <Wire.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

unsigned long lastMillis = 0;
const int renderIntervalMs = 50;

unsigned long lastStepTime = 0;

EncoderHelper Encoder(CLK_PIN, DT_PIN, SW_PIN, STEPS_PER_CLICK);

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

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Servo tester");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Servo");
  display.println("Tester");
  display.display();

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
}

void loop() {
  Encoder.update();
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= renderIntervalMs) {
    lastMillis = currentMillis;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.println(displayAnge(isPrimaryAngleActive, "A", angleA));
    display.println(displayAnge(!isPrimaryAngleActive, "B", angleB));
    display.display();
  }

  angle = isPrimaryAngleActive ? angleA : angleB;
  testServo.write(angle);
}
