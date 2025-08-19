#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // I2C address 0x78
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello OLED!");
  display.display();
}

int counter = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 50;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    counter++;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Counter: ");
    display.println(counter);
    display.println("");
    display.println("millis");
    display.println(currentMillis);
    display.display();
  }
}
