#include <Arduino.h>
#include <DisplayHelper.h>
#include <Wire.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

DisplayHelper::DisplayHelper()
    : lastMillis(0), display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void DisplayHelper::init() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void DisplayHelper::update() {
  unsigned long currentMillis = millis();
  unsigned long renderIntervalMs = 50;

  if (currentMillis - lastMillis >= renderIntervalMs) {
    lastMillis = currentMillis;
    display.clearDisplay();
    display.setCursor(0, 0);

    if (onRender) {
      onRender(display);
    }

    display.display();
  }
}
