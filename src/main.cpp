#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Encoder.h>
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
Encoder encoder(CLK_PIN, DT_PIN);
long position = 0;
long lastPosition = 0;
byte angle = 0;
byte angleA = 0;
byte angleB = 0;
boolean isPrimaryAngleActive = true;

unsigned long lastMillis = 0;
const int renderIntervalMs = 50;
const int debounceIntervalMs = 200;
unsigned long lastDebounceMillis = 0;
bool lastButtonState = HIGH;

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

String displayAnge(boolean active, String label, int angle) {
  const byte angleStringTargetLen = 3;
  String output = "";
  if (active) {
    output += "[";
    output += label;
    output += "] ";
  } else {
    output += " ";
    output += label;
    output += "  ";
  }

  String angleStr = String(angle);
  while (angleStr.length() < angleStringTargetLen)
    angleStr = " " + angleStr;

  output += angleStr + "deg";
  return output;
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

  if (lastButtonState == LOW && buttonState == HIGH &&
      currentMillis - lastDebounceMillis >= debounceIntervalMs) {
    lastDebounceMillis = currentMillis;
    isPrimaryAngleActive = !isPrimaryAngleActive;

    Serial.println("Switch between A and B");
  }

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
  lastButtonState = buttonState;
}
