#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Encoder.h>
#include <Servo.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define STEPS_PER_CLICK 4

#define TEST_SERVO_PIN 9
Servo testServo;
Encoder encoder(CLK_PIN, DT_PIN);
long position = 0;
long lastPosition = 0;
byte angle = 0;
byte angleA = 0;
byte angleB = 0;
boolean isPrimaryAngleActive = true;

unsigned long lastMillis = 0;
const int intervalMs = 50;
bool lastButtonState = HIGH;

// for momentum
unsigned long lastStepTime = 0;

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Servo tester");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Counter: ");
  display.println(position);
  display.display();

  testServo.attach(TEST_SERVO_PIN);
  testServo.write(0);
}

void loop() {
  long newPosition = encoder.read() / STEPS_PER_CLICK;
  unsigned long currentMillis = millis();
  bool buttonState = digitalRead(SW_PIN);

  if (newPosition != lastPosition) {
    int direction = (newPosition < lastPosition) ? 1 : -1;

    unsigned long deltaT = currentMillis - lastStepTime;
    lastStepTime = currentMillis;

    int step = 1;
    if (deltaT < 50)
      step = 6;
    else if (deltaT < 100)
      step = 3;
    else if (deltaT < 200)
      step = 2;
    else
      step = 1;

    byte newAngle = constrain(angle + step * direction, 0, 180);
    if (isPrimaryAngleActive) {
      angleA = newAngle;
    } else {
      angleB = newAngle;
    }
    position = constrain(newPosition, 0, 180);
    lastPosition = newPosition;
  }

  if (lastButtonState == HIGH && buttonState == LOW) {
    isPrimaryAngleActive = !isPrimaryAngleActive;

    Serial.println("Reset counter");
  }

  if (currentMillis - lastMillis >= intervalMs) {
    lastMillis = currentMillis;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Selection: ");
    display.println(isPrimaryAngleActive ? "A" : "B");
    display.println();
    display.print("Angle: ");
    display.print(angle);
    display.println("deg");
    display.display();
  }

  angle = isPrimaryAngleActive ? angleA : angleB;
  testServo.write(angle);
  lastButtonState = buttonState;
}
