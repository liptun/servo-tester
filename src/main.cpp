#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Encoder.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define STEPS_PER_CLICK 2

Encoder encoder(CLK_PIN, DT_PIN);
long position = 0;
long lastPosition = 0;
int counter = 0;

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
    if (deltaT < 10)
      step = 10; // super fast spin
    else if (deltaT < 30)
      step = 5; // fast
    else if (deltaT < 100)
      step = 2; // medium
    else
      step = 1; // slow

    counter += direction * step;
    position = newPosition;
    lastPosition = newPosition;

    Serial.print("Counter: ");
    Serial.println(counter);
  }

  if (lastButtonState == HIGH && buttonState == LOW) {
    counter = 0;

    Serial.println("Reset counter");
  }

  if (currentMillis - lastMillis >= intervalMs) {
    lastMillis = currentMillis;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Counter: ");
    display.println(counter);
    display.display();
  }

  lastButtonState = buttonState;
}
